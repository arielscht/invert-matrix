ROOT_LKD_COMMAND = 'likwid-perfctr'
ROOT_CSV_DIR = 'csv'
ROOT_LOG_DIR = 'logs'
ROOT_PDF_DIR = 'pdfs'
VERSIONS  = [{ "version": 'v1', "exec": "v1"}, { "version": 'v2', "exec": "v2" }]
MATRIX_SIZES = [32, 33, 64, 65, 128, 129, 256, 257, 512, 1000, 2000, 4000, 6000, 10000]
INTERESTED_GROUPS = ["L3", "L2CACHE", "FLOPS_DP"]
INTERESTED_DATA = ["L3", "L2CACHE", "FLOPS_DP", "TIME"]
INTERESTED_PATTERNS = {
  "L3": "(L3 bandwidth \[?MBytes\/s]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "L2CACHE": "(L2 miss ratio *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "FLOPS_DP": "(  DP \[?MFLOP/s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  # "FLOPS_AVX": "(Packed SP \[?MFLOP/s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "TIME": "(RDTSC Runtime \[?s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)"
}
LABELS = {
  "L3": "L3 bandwidth MBytes/s",
  "L2CACHE": "L2 miss ratio",
  "FLOPS_DP": "DP MFLOP/s",
  "FLOPS_AVX": "AVX MFLOP/s",
  "TIME": "seconds"
}

