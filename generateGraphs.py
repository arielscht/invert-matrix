import csv
import os
import commonConstants as constants
import matplotlib.pyplot as plt

def handleExistenceNecessaryDirs():
  pdfPath = f'./{constants.ROOT_PDF_DIR}'
  if(not os.path.exists(pdfPath)):
    os.mkdir(pdfPath)

def buildPdfPath(group):
  return f'./{constants.ROOT_PDF_DIR}/{str(group)}.pdf'

def buildCsvPath(version, group, operation):
  return f'./{constants.ROOT_CSV_DIR}/{str(version)}/{str(group)}{"-"}OP{str(operation)}.csv'

def checkExistenceNecessaryDirs():
  logPath = f'./{constants.ROOT_CSV_DIR}'
  if(not os.path.exists(logPath)):
    return False
  for version in constants.VERSIONS:
    versionPath = os.path.join(logPath, version["version"])
    if(not os.path.exists(versionPath)):
      return False
  return True

if(not checkExistenceNecessaryDirs()):
  print("Before running this script, it need to have the necessary dirs")
  exit()

handleExistenceNecessaryDirs()

for group in constants.INTERESTED_DATA:
  for version in constants.VERSIONS:
    for operation in [1, 2]:
      csvPath = buildCsvPath(version['version'], group, operation)
      if not os.path.exists(csvPath): continue

      with open(csvPath, newline='') as file:
        reader = csv.DictReader(file)
        xData = []
        yData = []

        for row in reader:
          size = row['Size']
          data = row['Data']
          if not size or not data: continue

          xData.append(int(size))
          yData.append(float(data))

      plt.plot(xData, yData, label = f"OP{operation} - {version['version']}", marker='o')
  plt.yscale('log')
  plt.xlabel('Matrix size')
  plt.ylabel(constants.LABELS[group])
  plt.title(f"Graph for {group}")
  plt.legend()
  plt.savefig(buildPdfPath(group))
  plt.clf()



