import os
import commonConstants as constants

os.system('make')

def handleExistenceNecessaryDirs():
  logPath = f'./{constants.ROOT_LOG_DIR}'
  if(not os.path.exists(logPath)):
    os.mkdir(logPath)

  for version in constants.VERSIONS:
    versionPath = os.path.join(logPath, version)
    if(not os.path.exists(versionPath)):
      os.mkdir(versionPath)

def buildLogPath(size, version, group):
  return f'./{constants.ROOT_LOG_DIR}/{str(version)}/{str(group)}{"-"}{str(size)}.log'

def buildProgramPath(size, itr = 10):
  return f'./invmat -r {str(size)} -i {str(itr)} -s /dev/null'

def buildLkdCommand(group, core = 3):
  return f'{constants.ROOT_LKD_COMMAND} -C {str(core)} -g {str(group)} -m'

handleExistenceNecessaryDirs()

for version in constants.VERSIONS:
  for size in constants.MATRIX_SIZES:
    for group in constants.INTERESTED_GROUPS:
      command = f'{buildLkdCommand(group)} {buildProgramPath(size)} {">"} {buildLogPath(size, version, group)}'
      os.system(command)