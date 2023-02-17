import { joi, Use, Context, createParamDecorator, Controller, Description, Get, QuerySchame, Ctx, Query, Post, Body, BodySchame } from 'koast';
import fs from 'fs';
import path from 'path';
const util = require('util')
import deccoder from '../utils/wav-deccoder';
import encoder from '../utils/wav-encoder';
import { floatTo16Bit, int16ToFloat32, average } from '../utils/tool';
import send from 'koa-send'
import multer from '@koa/multer'
import WavModel from '../model/wav.model'
const stat = util.promisify(fs.stat)

const upload = multer();

@Controller('/wav')
export default class Wav {
  @Get('/page')
  @QuerySchame({
    page: joi.string().default('1'),
    size: joi.string().default('10'),
  })
  async page(@Ctx() ctx: Context, @Query() query: any) {
    const { data, total } = await WavModel.page(query.page, query.size)
    return { code: 0, data: { data, total } }
  }

  @Get('/info')
  @QuerySchame({
    id: joi.string().required()
  })
  async info(@Ctx() ctx: Context, @Query() query: any) {
    const id = query.id
    const wav = await WavModel.getById(id)
    if (!wav) {
      return { code: 40004, msg: '404' }
    }
    return { code: 0, data: wav }
  }

  @Get('/raw')
  @QuerySchame({
    id: joi.string().required()
  })
  async raw(@Ctx() ctx: Context, @Query() query: any) {
    const id = query.id
    const wav = await WavModel.getById(id)
    if (!wav) {
      return { code: 40004, msg: '404' }
    }
    const p = path.join(wav.directory, wav.wav_name + '.wav')
    const stats = await stat(p)
    ctx.set('Content-Length', stats.size)
    ctx.set('Content-Type', 'audio/wave')
    ctx.body = fs.createReadStream(p)
  }
}
