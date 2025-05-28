import fs from 'fs';

class BuffersManager {
  inBuffer: string;
  outFile: string;
  MATCH_LENGTH: number;
  matcher: RegExp;
  filter: string;
  textInsertionStart: string;
  textInsertionEnd: string;
  constructor({
    inBuffer,
    outFile,
    matchLength,
    matcher,
    filter = '',
    textInsertionStart = '',
    textInsertionEnd = '',
    writeToFile = false,
  } : {
    inBuffer: string,
    outFile: string,
    matchLength: number,
    matcher: RegExp,
    filter: string,
    textInsertionStart: string,
    textInsertionEnd: string,
    writeToFile: boolean,
  }) {
    this.inBuffer = inBuffer;
    this.outFile = outFile;
    this.MATCH_LENGTH = matchLength;
    this.matcher = matcher;
    this.filter = filter,
    this.textInsertionStart = textInsertionStart;
    this.textInsertionEnd = textInsertionEnd;
    this.processBuffersForMatches();
    if (writeToFile) this.writeBufferToFile();
  }

  matchBuffer: string[] = [];
  matchInfo: (RegExpMatchArray | null) = null;
  matchCount: number = 0;
  outBuffer: string = '';
  strTmp: string = '';

  addMatch(str: string) {
    this.matchBuffer.push(str);
  }

  shiftMatch(straightToBuffer = true) {
    if (this.matchBuffer.length === 0) return;
    if (straightToBuffer) {
      this.outBuffer += this.matchBuffer.shift();
      return;
    }
    this.strTmp += this.matchBuffer.shift();
  }

  addOut(str: string) {
    this.outBuffer += str;
  }

  flushMatchBuffer(straightToBuffer = false) {
    if (this.filter === '') {
      straightToBuffer = true;
    }
    while (this.matchBuffer.length > 0) {
      this.shiftMatch(straightToBuffer);
    }
    if (straightToBuffer) return;
    this.strTmp = this.strTmp.replace(this.filter, '');
    this.outBuffer += this.strTmp;
    this.strTmp = '';
  }

  checkMatch() {
    this.matchInfo = this.matchBuffer.join('').match(this.matcher);
  }

  insertText(str: string) {
    if (this.matchCount == 0) {
      this.outBuffer += this.textInsertionStart;
      this.flushMatchBuffer();
      this.addMatch(str);
      return;
    }
    this.flushMatchBuffer();
    this.addMatch(str);
  }

  endTextInsertion() {
    this.outBuffer += this.textInsertionEnd;
    this.matchCount = 0;
  }

  handleMatch(str: string) {
    this.insertText(str);
    this.matchCount++;
  }

  handleBufferEnd() {
    if (this.matchCount > 0) this.outBuffer += this.textInsertionEnd;
    this.flushMatchBuffer(true);
  }

  processBuffersForMatches() {
    for (let i = 0; i < this.inBuffer.length; i++) {
      if (this.matchBuffer.length < this.MATCH_LENGTH) {
        this.addMatch(this.inBuffer[i]);
        continue;
      }

      this.checkMatch();
      if (this.matchInfo) {
        // NOTE: handleMatch/insertText calls flushMatchBuffer
        this.handleMatch(this.inBuffer[i]);
        continue;
      } else if (!this.matchInfo && this.matchCount > 0) {
        this.endTextInsertion();
      }

      this.shiftMatch();
      this.addMatch(this.inBuffer[i]);
    }
    // NOTE: handleBufferEnd calls flushMatchBuffer
    this.handleBufferEnd();
  }

  writeBufferToFile() {
    fs.writeFileSync(this.outFile, this.outBuffer);
  }
};

export { BuffersManager };
