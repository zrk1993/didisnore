import fs from 'fs';
import dayjs from 'dayjs'
import path from 'path';
import { getHeader } from './utils/wav-encoder';
import WavModel from './model/wav.model'

let lastTime = Date.now();
let cahce = []

export default async (data: Buffer) => {
  if (cahce.length > 235) {
    const d = Buffer.concat(cahce)
    const h = getHeader(d.length);
    const data = Buffer.concat([h, d]);
    save(data)
    cahce = []
    lastTime = Date.now();
  }
  cahce.push(data)
}

async function save(data: Buffer) {
  const savePath = path.join(__dirname, '..', 'upload')
  const directory = dayjs(lastTime).format("YYMMDD")
  if (!fs.existsSync(path.join(savePath, directory))) {
    fs.mkdirSync(path.join(savePath, directory), { recursive: true })
  }
  const wav_name = dayjs(lastTime).format("YYMMDD_HHmmssSSS")
  fs.open(path.join(savePath, directory, `${wav_name}.wav`), 'w', (err, fd) => {
    if (err) {
      console.error(err)
    }
    fs.write(fd, data, 0, data.length, 0, (err, bytesWritten, buffer) => {
      fs.fsync(fd, err => {
        fs.close(fd, err => {
          console.log("关闭文件");
          WavModel.insert({
            wav_name,
            duration: Math.floor(data.length / 32000),
            create_time: new Date(),
            directory: path.join(savePath, directory)
          })
        });
      });
    });
  });
}