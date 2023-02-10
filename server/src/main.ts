import path from 'path';
import * as net from 'net';
import { Koast } from 'koast';
import koaStatic from 'koa-static';
import router from './router';

import dataHand from './tcp-data-hand'

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

  const tcp_server = net.createServer((client) => {
    console.log('client')
    client.on('close', () => {
      console.log('close')
    });
    client.on('error', () => {
      console.log('error')
    });
    client.on('end', () => {
      console.log('end')
    });
    client.on('data', (data) => {
      //console.log('.');
      dataHand(data);
    });
  })
  tcp_server.listen(3006, () => {
    console.log('TCP服务器已启动');
  })
}

main()
