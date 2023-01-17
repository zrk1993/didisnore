import { joi, Use, Context, createParamDecorator, Controller, Description, Get, QuerySchame, Ctx, Query, Post, Body, BodySchame } from 'koast';


@Controller('/user')
@Description('user')
export default class Test {
  @Post('/login')
  async login(@Ctx() ctx: Context, @Body() body: any) {
    const { username, password } = body
    if (username === 'admin' && password === '123456789') {
      return { code: 0, data: { token: 'token' } };
    } else {
      return { code: 5000, message: '账号或密码错误！' };
    }
  }

  @Get('/info')
  async info(@Ctx() ctx: Context, @Query() query: any) {
    return { code: 0, data: {
      username: 'admin',
      roles: ['admin']
    } };
  }
}
