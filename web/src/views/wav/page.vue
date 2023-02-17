<template>
  <el-table :data="tableData" style="width: 100%">
    <el-table-column prop="date" label="Date" width="180" />
    <el-table-column prop="name" label="Name" width="180" />
    <el-table-column prop="address" label="Address" />
  </el-table>
</template>

<script lang="ts" setup>
import { reactive, ref } from "vue"
import { tableData } from "@/api/wav"

const tableData = reactive([])
const pageInfo = reactive({
  page: 1,
  size: 10,
  total: 0
})

async function getWav() {
  const { code, data } = await tableData(pageInfo)
}
</script>

<style lang="scss" scoped>
.center {
  height: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
}
</style>