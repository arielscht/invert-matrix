import re
import os
import csv
import commonConstants as constants

def handleExistenceNecessaryDirs():
  csvPath = f'./{constants.ROOT_CSV_DIR}'
  if(not os.path.exists(csvPath)):
    os.mkdir(csvPath)

  for version in constants.VERSIONS:
    versionPath = os.path.join(csvPath, version)
    if(not os.path.exists(versionPath)):
      os.mkdir(versionPath)

def checkExistenceNecessaryDirs():
  logPath = f'./{constants.ROOT_LOG_DIR}'
  if(not os.path.exists(logPath)):
    return False
  for version in constants.VERSIONS:
    versionPath = os.path.join(logPath, version)
    if(not os.path.exists(versionPath)):
      return False
  return True

def buildCsvPath(version, group, operation):
  return f'./{constants.ROOT_CSV_DIR}/{str(version)}/{str(group)}{"-"}OP{str(operation)}.csv'

def buildLogPath(size, version, group):
  return f'./{constants.ROOT_LOG_DIR}/{str(version)}/{str(group)}{"-"}{str(size)}.log'

def writeCsv(filepath, header, data):
  with open(filepath, 'w', encoding='UTF8', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(header)
    writer.writerows(data)

if(not checkExistenceNecessaryDirs()):
  print("Before running this script, it need to have the necessary dirs")
  exit()

handleExistenceNecessaryDirs()

for version in constants.VERSIONS:
  for group in constants.INTERESTED_DATA:
    dataOperationOne = []
    dataOperationTwo = []
    for size in constants.MATRIX_SIZES:
      logPath = buildLogPath(size, version, group if group != "TIME" else "FLOPS_DP")
      if not os.path.exists(logPath): continue
      file = open(logPath, 'r')
      fileContent = file.read()
      file.close()
      result = re.findall(constants.INTERESTED_PATTERNS[group], fileContent)
      if len(result) != 2: continue
      dataOperationOne.append([size, result[0][1]])
      dataOperationTwo.append([size, result[1][1]])

    operationOnePath = buildCsvPath(version, group, 1)
    operationTwoPath = buildCsvPath(version, group, 2)
    writeCsv(operationOnePath, ["Size", "Data"], dataOperationOne)
    writeCsv(operationTwoPath, ["Size", "Data"], dataOperationTwo)