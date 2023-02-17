import BaseModel from './base.model';

const tableName = 't_wav';

interface ITableStructure {
  admin_id?: number,
  uname?: string,
  [propname: string]: any
};

class WavModel extends BaseModel<ITableStructure> {

  constructor() {
    super({ tableName });
  }

  async getByName(uname: string): Promise<ITableStructure> {
    return this.$db.table(tableName).where({ uname }).findOrEmpty();
  }

}

export default new WavModel();

