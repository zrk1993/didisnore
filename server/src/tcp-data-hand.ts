import fs from 'fs';
import dayjs from 'dayjs'
import path from 'path';
import { getHeader } from './utils/wav-encoder';

let lastTime = Date.now();
let cahce = []

export default async (data: Buffer) => {
  if (cahce.length > 1000) {
    const d = Buffer.concat(cahce)
    const h = getHeader(d.length);
    const data = Buffer.concat([h, d]);
    fs.open(`wav/upload/${dayjs(lastTime).format("YYMMDD_HHmmss_") + Math.ceil(d.length / 32000)}.wav`, 'w', (err, fd) => {
      if (err) {
        console.error(err)
      }
      // 读取 buf 向文件写入数据
      fs.write(fd, data, 0, data.length, 0, (err, bytesWritten, buffer) => {
        // 同步磁盘缓存
        fs.fsync(fd, err => {
          // 关闭文件
          fs.close(fd, err => {
            console.log("关闭文件");
          });
        });
      });
    });
    cahce = []
    lastTime = Date.now();
  }
  cahce.push(data)
}