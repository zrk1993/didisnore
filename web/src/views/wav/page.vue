<template>
  <div class="p15">
    <el-table v-loading="loading" :data="tableData" style="width: 100%">
      <el-table-column prop="id" label="Id" />
      <el-table-column prop="wav_name" label="音频">
        <template #default="{ row }">
          <span class="cursor-pointer" @click="goDetail(row.id)">{{ row.wav_name }}</span>
        </template>
      </el-table-column>
      <el-table-column prop="create_time" label="时间">
        <template #default="{ row }">{{ dayjs(row.create_time).format('MM-DD HH:mm:ss') }}</template>
      </el-table-column>
      <el-table-column prop="duration" label="时长" />
    </el-table>
    <div class="flex justify-content-center mt15">
      <el-pagination background layout="prev, pager, next" @current-change="currentChange" :current-page="pageInfo.page" :total="pageInfo.total" />
    </div>
  </div>
</template>

<script lang="ts" setup>
import dayjs from "dayjs";
import { reactive, ref, onMounted } from "vue"
import { useRoute, useRouter } from 'vue-router';
import { getWavPage } from "@/api/wav"
import { tr } from "element-plus/es/locale";

let loading = ref(false)
let tableData = reactive([])
const pageInfo = reactive({
  page: 1,
  size: 10,
  total: 0
})

const router = useRouter()

onMounted(() => {
  getTableData()
})

function goDetail(id: number) {
  router.push({ name: 'wav-detail', params: { id } })
}

async function currentChange(v: number) {
  pageInfo.page = v
  getTableData()
}

async function getTableData() {
  loading.value = true
  try {
    const { code, data } = await getWavPage(pageInfo)
    if (code !== 0) {
      return
    }
    tableData = data.data
    pageInfo.total = data.total
  } catch (error) {
    console.error(error)
  } finally {
    loading.value = false
  }
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
