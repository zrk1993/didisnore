/**
 * @Author: kun
 */

import { DBM } from 'soul-orm';
import config from '../config'

const orm = new DBM({
  connectionLimit: 10,
  host: config.db.host,
  port: config.db.port,
  user: config.db.user,
  password: config.db.password,
  database: config.db.database,
  isDebug: false,
});

export default orm;
