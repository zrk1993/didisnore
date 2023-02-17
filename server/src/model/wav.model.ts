import BaseModel from './base.model';

const tableName = 'wav';

interface ITableStructure {
  id?: number,
  wav_name: string,
  create_time: Date,
  directory: string,
  duration: number,
  [propname: string]: any
};

class WavModel extends BaseModel<ITableStructure> {

  constructor() {
    super({ tableName, primaryKey: 'id' });
  }

  async getByName(uname: string): Promise<ITableStructure> {
    return this.$db.table(tableName).where({ uname }).findOrEmpty();
  }

}

export default new WavModel();

