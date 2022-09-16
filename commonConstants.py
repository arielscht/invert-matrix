ROOT_LKD_COMMAND = 'likwid-perfctr'
ROOT_CSV_DIR = 'csv'
ROOT_LOG_DIR = 'logs'
ROOT_PDF_DIR = 'pdfs'
VERSIONS  = [{ "version": 'v1', "exec": "v1"}, { "version": 'v2', "exec": "v2" }] #Versões do código
MATRIX_SIZES = [32, 33, 64, 65, 128, 129, 256, 257, 512] #Limite de tamanho de matrizes para o programa executar
INTERESTED_GROUPS = ["L3", "L2CACHE", "FLOPS_DP"] #Vetor de contadores do LIKWID para a execução do programa
INTERESTED_DATA = ["L3", "L2CACHE", "FLOPS_DP", "FLOPS_AVX", "TIME"] #Grupos de interesse para a geração dos gráficos
INTERESTED_PATTERNS = {
  "L3": "(L3 bandwidth \[?MBytes\/s]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "L2CACHE": "(L2 miss ratio *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "FLOPS_DP": "(  DP \[?MFLOP/s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "FLOPS_AVX": "(AVX DP \[?MFLOP/s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)",
  "TIME": "(RDTSC Runtime \[?s\]? *\| *)([0-9]+\.?[0-9]*|\.[0-9]+)"
} #Regex para obter os dados de interesse

LABELS = {
  "L3": "L3 bandwidth MBytes/s",
  "L2CACHE": "L2 miss ratio",
  "FLOPS_DP": "DP MFLOP/s",
  "FLOPS_AVX": "AVX MFLOP/s",
  "TIME": "seconds"
} # labels para o eixo y dos gráficos gerados

