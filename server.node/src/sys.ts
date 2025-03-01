enum LogLevel {
  INFO = "INFO",
  WARN = "WARN",
  ERROR = "ERROR",
}

function Log(msg: string, logLevel = LogLevel.INFO): void {
  const _msg = `${DateTime_UTCp8()} [${logLevel}] ${msg}`;
  let loggger = console.log;
  if(logLevel === LogLevel.ERROR)
    loggger = console.error;
  loggger(_msg);
}

function DateTime_UTCp8(): string {
  const now = new Date();
  now.setHours(now.getHours() + 8);
  const year = now.getFullYear();
  const month = String(now.getMonth() + 1).padStart(2, "0");
  const day = String(now.getDate()).padStart(2, "0");
  const hours = String(now.getHours()).padStart(2, "0");
  const minutes = String(now.getMinutes()).padStart(2, "0");
  const seconds = String(now.getSeconds()).padStart(2, "0");

  return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

function RandInt(lowerLimit: number, upperLimit: number): number {
  if (!Number.isInteger(lowerLimit) || !Number.isInteger(upperLimit)) {
    throw new Error("Both limits must be integers");
  }
  if (lowerLimit > upperLimit) {
    throw new Error("Lower limit must be less than or equal to upper limit");
  }
  return Math.floor(Math.random() * (upperLimit - lowerLimit + 1)) + lowerLimit;
}

export { Log, DateTime_UTCp8, RandInt, LogLevel };
