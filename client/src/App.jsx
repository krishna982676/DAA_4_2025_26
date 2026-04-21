import { useEffect, useMemo, useRef, useState } from "react";
import { Download, Sparkles, Zap } from "lucide-react";
import UploadPanel from "./components/UploadPanel";
import ControlPanel from "./components/ControlPanel";
import PreviewPanel from "./components/PreviewPanel";
import StatsDashboard from "./components/StatsDashboard";
import AnalysisPanel from "./components/AnalysisPanel";
import ProgressBanner from "./components/ProgressBanner";

const API_ENDPOINTS = {
  estimate: "/api/estimate",
  compress: "/api/compress",
  decompress: "/api/decompress",
  optimize: "/api/optimize"
};

function formatBytes(bytes) {
  if (!Number.isFinite(bytes)) {
    return "-";
  }

  if (bytes < 1024) {
    return `${bytes.toFixed(0)} B`;
  }

  if (bytes < 1024 * 1024) {
    return `${(bytes / 1024).toFixed(2)} KB`;
  }

  return `${(bytes / (1024 * 1024)).toFixed(2)} MB`;
}

function downloadBase64(base64, fileName, mimeType) {
  const anchor = document.createElement("a");
  anchor.href = `data:${mimeType};base64,${base64}`;
  anchor.download = fileName;
  anchor.click();
}

function makePayload(selectedFile, controls) {
  const payload = new FormData();
  payload.append("image", selectedFile);
  payload.append("mode", controls.mode);
  payload.append("colorMode", controls.colorMode);
  payload.append("targetType", controls.targetType);
  payload.append("targetValue", String(controls.targetValue));
  payload.append("quality", String(controls.quality));
  return payload;
}

function App() {
  const [selectedFile, setSelectedFile] = useState(null);
  const [isDragging, setIsDragging] = useState(false);
  const [originalPreview, setOriginalPreview] = useState("");
  const [reconstructedPreview, setReconstructedPreview] = useState("");
  const [optimizedPreview, setOptimizedPreview] = useState("");

  const [controls, setControls] = useState({
    mode: "lossy",
    colorMode: "rgb",
    targetType: "percentage",
    targetValue: 40,
    quality: 72
  });

  const [estimate, setEstimate] = useState(null);
  const [compressResult, setCompressResult] = useState(null);
  const [optimizeResult, setOptimizeResult] = useState(null);

  const [busy, setBusy] = useState(false);
  const [busyStage, setBusyStage] = useState("Waiting...");
  const [progress, setProgress] = useState(0);
  const [statusMessage, setStatusMessage] = useState("Upload an image to start compression analysis.");

  const progressTimerRef = useRef(null);

  function startProgress(stageText) {
    setBusy(true);
    setBusyStage(stageText);
    setProgress(6);

    progressTimerRef.current = window.setInterval(() => {
      setProgress((prev) => Math.min(prev + Math.random() * 10, 90));
    }, 220);
  }

  function stopProgress() {
    if (progressTimerRef.current) {
      window.clearInterval(progressTimerRef.current);
      progressTimerRef.current = null;
    }

    setProgress(100);
    window.setTimeout(() => {
      setBusy(false);
      setProgress(0);
    }, 220);
  }

  function updateControl(name, value) {
    setControls((prev) => ({
      ...prev,
      [name]: ["targetValue", "quality"].includes(name) ? Number(value) : value
    }));
  }

  function handleFile(file) {
    if (!file) {
      return;
    }

    if (!file.type.startsWith("image/")) {
      setStatusMessage("Please upload PNG or JPG images only.");
      return;
    }

    setSelectedFile(file);
    setCompressResult(null);
    setOptimizeResult(null);
    setReconstructedPreview("");
    setOptimizedPreview("");

    const localURL = URL.createObjectURL(file);
    setOriginalPreview(localURL);
    setStatusMessage(`File selected: ${file.name}`);
  }

  async function fetchEstimate(file, currentControls) {
    const payload = makePayload(file, currentControls);
    const response = await fetch(API_ENDPOINTS.estimate, {
      method: "POST",
      body: payload
    });

    const result = await response.json();
    if (!response.ok) {
      throw new Error(result.message || "Failed to estimate compression size.");
    }

    setEstimate({
      ...result,
      estimatedSizeLabel: formatBytes(result.estimatedBytes),
      estimatedReductionLabel: `${result.estimatedReductionPercent.toFixed(2)}%`
    });
  }

  async function handleCompress() {
    if (!selectedFile) {
      setStatusMessage("Upload an image before compression.");
      return;
    }

    startProgress("Compressing image with adaptive Huffman strategy...");

    try {
      const payload = makePayload(selectedFile, controls);

      const response = await fetch(API_ENDPOINTS.compress, {
        method: "POST",
        body: payload
      });

      const result = await response.json();
      if (!response.ok) {
        throw new Error(result.message || "Compression failed.");
      }

      setCompressResult(result);
      setStatusMessage(
        `Compression complete using ${result.header.strategy}. Final size: ${formatBytes(
          result.stats.finalFileBytes
        )}.`
      );
    } catch (error) {
      setStatusMessage(error.message);
    } finally {
      stopProgress();
    }
  }

  async function handleDecompress() {
    if (!compressResult?.compressedBase64) {
      setStatusMessage("Compress a file first.");
      return;
    }

    startProgress("Reconstructing image from compressed output...");

    try {
      const response = await fetch(API_ENDPOINTS.decompress, {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify({ compressedBase64: compressResult.compressedBase64 })
      });

      const result = await response.json();
      if (!response.ok) {
        throw new Error(result.message || "Decompression failed.");
      }

      setReconstructedPreview(`data:${result.mimeType};base64,${result.reconstructedImageBase64}`);
      setStatusMessage("Decompression complete. Reconstructed image is ready for comparison.");
    } catch (error) {
      setStatusMessage(error.message);
    } finally {
      stopProgress();
    }
  }

  async function handleOptimizeImage() {
    if (!selectedFile) {
      setStatusMessage("Upload an image before optimization.");
      return;
    }

    startProgress("Generating optimized downloadable image...");

    try {
      const payload = new FormData();
      payload.append("image", selectedFile);
      payload.append("format", controls.mode === "lossless" ? "jpeg" : "webp");
      payload.append("quality", String(controls.quality));

      const response = await fetch(API_ENDPOINTS.optimize, {
        method: "POST",
        body: payload
      });

      const result = await response.json();
      if (!response.ok) {
        throw new Error(result.message || "Optimization failed.");
      }

      setOptimizeResult(result);
      setOptimizedPreview(`data:${result.mimeType};base64,${result.optimizedImageBase64}`);
      setStatusMessage(
        `Optimized image created. Reduction: ${result.stats.reductionPercent.toFixed(2)}% (${formatBytes(
          result.stats.optimizedBytes
        )}).`
      );
    } catch (error) {
      setStatusMessage(error.message);
    } finally {
      stopProgress();
    }
  }

  function handleDragOver(event) {
    event.preventDefault();
    setIsDragging(true);
  }

  function handleDragLeave(event) {
    event.preventDefault();
    setIsDragging(false);
  }

  function handleDrop(event) {
    event.preventDefault();
    setIsDragging(false);
    handleFile(event.dataTransfer.files?.[0]);
  }

  useEffect(() => {
    if (!selectedFile) {
      setEstimate(null);
      return undefined;
    }

    const timer = window.setTimeout(() => {
      fetchEstimate(selectedFile, controls).catch((error) => {
        setEstimate(null);
        setStatusMessage(error.message);
      });
    }, 420);

    return () => window.clearTimeout(timer);
  }, [selectedFile, controls]);

  useEffect(() => {
    return () => {
      if (progressTimerRef.current) {
        window.clearInterval(progressTimerRef.current);
      }
    };
  }, []);

  const statsForUi = useMemo(() => {
    if (!compressResult?.stats) {
      return null;
    }

    const stats = compressResult.stats;

    return {
      originalSizeLabel: formatBytes(stats.originalBytes),
      encodedBitsLabel: `${stats.encodedBitSize.toLocaleString()} bits`,
      encodedBytesLabel: `Packed: ${formatBytes(stats.encodedBytes)}`,
      metadataLabel: formatBytes(stats.metadataBytes),
      finalSizeLabel: formatBytes(stats.finalFileBytes),
      ratioLabel: `${stats.ratio.toFixed(3)} : 1`,
      reductionLabel: `${stats.compressionPercent.toFixed(2)} %`
    };
  }, [compressResult]);

  return (
    <div className="min-h-screen px-4 py-6 md:px-8">
      <div className="mx-auto max-w-7xl space-y-4">
        <header className="animate-riseIn rounded-3xl border border-white/70 bg-white/80 p-6 shadow-panel backdrop-blur-md">
          <div className="flex flex-wrap items-center justify-between gap-4">
            <div>
              <p className="inline-flex items-center gap-2 rounded-full bg-cyan-100 px-3 py-1 text-xs font-semibold text-cyan-800">
                <Sparkles className="h-3 w-3" />
                Professional Compression Studio
              </p>
              <h1 className="mt-2 font-display text-3xl font-extrabold tracking-tight text-slate-900 md:text-4xl">
                Huffman Image Compressor Pro
              </h1>
              <p className="mt-2 max-w-3xl text-sm text-slate-600 md:text-base">
                Hybrid lossless and lossy workflow with target controls, accurate metadata-aware stats, and
                interactive Huffman analysis built for DAA submission and portfolio demos.
              </p>
            </div>

            <button
              type="button"
              className="btn btn-secondary"
              disabled={!compressResult?.compressedBase64}
              onClick={() =>
                downloadBase64(
                  compressResult.compressedBase64,
                  compressResult.compressedFileName || "compressed.huffimg",
                  "application/octet-stream"
                )
              }
            >
              <Download className="h-4 w-4" />
              Download .huffimg
            </button>
          </div>
        </header>

        <ProgressBanner busy={busy} stage={busyStage} progress={progress} />

        <div className="grid gap-4 xl:grid-cols-2">
          <UploadPanel
            file={selectedFile}
            statusMessage={statusMessage}
            onSelectFile={handleFile}
            onDrop={handleDrop}
            onDragOver={handleDragOver}
            onDragLeave={handleDragLeave}
            isDragging={isDragging}
            busy={busy}
          />

          <ControlPanel
            controls={controls}
            onControlChange={updateControl}
            onCompress={handleCompress}
            onDecompress={handleDecompress}
            onOptimize={handleOptimizeImage}
            onDownloadCompressed={() =>
              downloadBase64(
                compressResult?.compressedBase64,
                compressResult?.compressedFileName || "compressed.huffimg",
                "application/octet-stream"
              )
            }
            busy={busy}
            canDecompress={Boolean(compressResult?.compressedBase64)}
            canDownloadCompressed={Boolean(compressResult?.compressedBase64)}
          />
        </div>

        <PreviewPanel
          originalPreview={originalPreview}
          reconstructedPreview={reconstructedPreview}
          optimizedPreview={optimizedPreview}
        />

        <StatsDashboard stats={statsForUi} estimate={estimate} />

        <AnalysisPanel analysis={compressResult?.analysis} />

        <footer className="pb-2 text-center text-xs text-slate-600">
          <p className="inline-flex items-center gap-1">
            <Zap className="h-3 w-3" />
            Greedy Huffman coding + adaptive preprocessing with precise storage accounting.
          </p>
        </footer>
      </div>
    </div>
  );
}

export default App;
