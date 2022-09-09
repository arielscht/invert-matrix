import os
import commonConstants as constants

os.system('cd ./v1 && make')
os.system('cd ..')
os.system('cd ./v2 && make')

def handleExistenceNecessaryDirs():
  logPath = f'./{constants.ROOT_LOG_DIR}'
  if(not os.path.exists(logPath)):
    os.mkdir(logPath)

  for version in constants.VERSIONS:
    versionPath = os.path.join(logPath, version["version"])
    if(not os.path.exists(versionPath)):
      os.mkdir(versionPath)

def buildLogPath(size, version, group):
  return f'./{constants.ROOT_LOG_DIR}/{str(version["version"])}/{str(group)}{"-"}{str(size)}.log'

def buildProgramPath(size, version, itr = 10):
  return f'./{str(version["exec"])}/invmat -r {str(size)} -i {str(itr)} -s /dev/null'

def buildLkdCommand(group, core = 3):
  return f'{constants.ROOT_LKD_COMMAND} -C {str(core)} -g {str(group)} -m'

handleExistenceNecessaryDirs()

for version in constants.VERSIONS:
  for size in constants.MATRIX_SIZES:
    for group in constants.INTERESTED_GROUPS:
      command = f'{buildLkdCommand(group)} {buildProgramPath(size, version)} {">"} {buildLogPath(size, version, group)}'
      os.system(command)