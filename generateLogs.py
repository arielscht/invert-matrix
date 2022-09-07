import os

# constants
ROOT_LKD_COMMAND = 'likwid-perfctr'
ROOT_LOG_DIR = 'logs'
VERSIONS  = ['v1']
MATRIX_SIZES = [32, 33, 64, 65, 128, 129, 256, 257, 512, 1000, 2000, 4000, 6000, 10000]
INTERESTED_GROUPS = ["L3", "ICACHE", "FLOPS_DP"]

os.system('make')

def handleExistenceNecessaryDirs():
  logPath = f'./{ROOT_LOG_DIR}'
  if(not os.path.exists(logPath)):
    os.mkdir(logPath)

  for version in VERSIONS:
    versionPath = os.path.join(logPath, version)
    if(not os.path.exists(versionPath)):
      os.mkdir(versionPath)

def buildLogPath(size, version, group):
  return f'./{ROOT_LOG_DIR}/{str(version)}/{str(group)}{"-"}{str(size)}.log'

def buildProgramPath(size, itr = 10):
  return f'./invmat -r {str(size)} -i {str(itr)} -s /dev/null'

def buildLkdCommand(group, core = 3):
  return f'{ROOT_LKD_COMMAND} -C {str(core)} -g {str(group)} -m'

handleExistenceNecessaryDirs()

for version in VERSIONS:
  for size in MATRIX_SIZES:
    for group in INTERESTED_GROUPS:
      command = f'{buildLkdCommand(group)} {buildProgramPath(size)} {">"} {buildLogPath(size, version, group)}'
      os.system(command)