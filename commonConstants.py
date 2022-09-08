ROOT_LKD_COMMAND = 'likwid-perfctr'
ROOT_CSV_DIR = 'csv'
ROOT_LOG_DIR = 'logs'
ROOT_PDF_DIR = 'pdfs'
VERSIONS  = [{ "version": 'v1', "exec": "./v1"}, { "version": 'v2', "exec": "." }]
MATRIX_SIZES = [32, 33, 64, 65, 128, 129, 256, 257]
INTERESTED_GROUPS = ["L3", "ICACHE", "FLOPS_DP"]
INTERESTED_DATA = ["L3", "ICACHE", "FLOPS_DP", "TIME"]
INTERESTED_PATTERNS = {
  "L3": "(L3 bandwidth \[?MBytes\/s]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "ICACHE": "(L1I miss ratio *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "FLOPS_DP": "(  DP \[?MFLOP/s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "TIME": "(RDTSC Runtime \[?s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)"
}
LABELS = {
  "L3": "L3 bandwidth MBytes/s",
  "ICACHE": "L1I miss ratio",
  "FLOPS_DP": "DP MFLOP/s",
  "TIME": "seconds"
}

