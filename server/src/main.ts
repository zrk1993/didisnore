import path from 'path';
import { Koast } from 'koast';
import koaStatic from 'koa-static';
import router from './router';

const port = 3005;

async function main() {
  const app = new Koast();
  app.useSwagger(router)
  console.log('swagger address http://localhost:' + port + '/swagger-ui/index.html')
  app.use(koaStatic(path.join(__dirname, '..', '/public')));
  app.useRouter(router);
  app.listen(port, () => {
    console.log('server start on http://localhost:' + port)
  });
}

main()
