import { joi, Use, Context, createParamDecorator, Controller, Description, Get, QuerySchame, Ctx, Query, Post, Body, BodySchame } from 'koast';
import fs from 'fs';
import path from 'path';
import deccoder from '../utils/wav-deccoder';
import { floatTo16Bit } from '../utils/tool';
import send from 'koa-send'

@Controller('/wav')
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

  @Get('/raw')
  async raw(@Ctx() ctx: Context, @Query() query: any) {
    await send(ctx, `wav/${query.id}.wav`)
  }
}
