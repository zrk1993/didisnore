import { joi, Use, Context, createParamDecorator, Controller, Description, Get, QuerySchame, Ctx, Query, Post, Body, BodySchame } from 'koast';
import fs from 'fs';
import path from 'path';
import deccoder from '../utils/wav-deccoder';
import encoder from '../utils/wav-encoder';
import { floatTo16Bit, int16ToFloat32, average } from '../utils/tool';
import send from 'koa-send'
import { koaBody } from 'koa-body'
import multer from '@koa/multer'

const upload = multer();

@Controller('/wav')
// @Use((ctx, next) => {
//   console.log(ctx.request.headers)
//   next()
// })
export default class Wav {
  @Get('/get')
  async hi(@Ctx() ctx: Context, @Query() query: any) {
    const b = await new Promise((resolve, reject) => {
      fs.readFile(path.join(__dirname, '../..', 'wav/snore.wav'), (err, data) => {
        if (err) {
          reject(err)
        };
        deccoder(data, {}).then((audioData: any) => {
          const i16 = floatTo16Bit(audioData.channelData[0])
          resolve({ code: 0, data: Array.from(i16) });
        })
      });
    })
    return b
  }

  @Post('/post')
  @Use(upload.single('file'))
  async update(@Ctx() ctx: Context) {
    const file: any = (ctx.request as any).file;
    let filePath = path.join(__dirname, '../..', 'wav/upload/') + `/${file.originalname}`;
    fs.writeFile(filePath, file.buffer, function () {
      console.log(filePath)
    });
    return {}
  }

  @Get('/raw')
  async raw(@Ctx() ctx: Context, @Query() query: any) {
    await send(ctx, `wav/${query.id}.wav`)
  }

  @Get('/raw2')
  async raw2(@Ctx() ctx: Context, @Query() query: any) {
    const b = await new Promise((resolve, reject) => {
      // 波形居中
      fs.readFile(path.join(__dirname, '../..', `wav/${query.id}.wav`), (err, data) => {
        if (err) {
          reject(err)
        };
        deccoder(data, {}).then((audioData: any) => {
          const bias = 0 - average(audioData.channelData[0].slice(0, Math.min(16000, audioData.channelData[0].length)))
          const whiteNoise1sec = {
            sampleRate: 16000,
            channelData: [
              audioData.channelData[0].map((v: number) => v + bias)
            ]
          };
          encoder(whiteNoise1sec, {}).then((buffer: any) => {
            fs.writeFileSync(`wav/${query.id}2.wav`, Buffer.from(buffer));
            resolve({ code: 0, data: [] });
          });
        })
      });
    })
    return b
  }
}
