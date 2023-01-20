<template>
  <div class="app-container">
    <MyWavesurfer :url="wavUrl"></MyWavesurfer>
    <div>
      <audio ref="audio" controls muted></audio>
    </div>
    <div>
      <el-button @click="record">录音</el-button>
      <el-button @click="stop">暂停</el-button>
      <el-button @click="downAll">下载</el-button>
    </div>
  </div>
</template>

<script setup lang="ts">
import MyWavesurfer from "@/components/MyWavesurfer/index.vue"
import ConcatenateBlobs from "@/utils/concatenateBlobs"
import MediaStreamRecorder from "msr"
import { onMounted, onUnmounted, ref } from "vue"

const wavUrl = ref('/api/wav/raw?id=40dB_gain2')
const audio = ref<any>(null)

let blobs: any[] = []
function down(blob: Blob) {
  const audioURL = window.URL.createObjectURL(blob)
  // 转换成文件
  const fileName = Date.now().toString(32)
  const fileType = "audio/wav"
  const file = new window.File([blob], fileName, { type: fileType })
  // 下载
  const a = document.createElement("a")
  a.href = audioURL
  a.download = fileName
  a.click()
}

function show() {
  ConcatenateBlobs(blobs, "audio/wav", (resultingBlob: Blob) => {
    var blobURL = URL.createObjectURL(resultingBlob)
  })
}

function downAll() {
  ConcatenateBlobs(blobs, "audio/wav", (resultingBlob: Blob) => {
    var blobURL = URL.createObjectURL(resultingBlob)
    down(resultingBlob)
  })
}

function captureUserMedia(mediaConstraints: any, successCallback: any, errorCallback: any) {
  navigator.mediaDevices.getUserMedia(mediaConstraints).then(successCallback).catch(errorCallback)
}

let mediaRecorder: any = null

function onMediaSuccess(stream: any) {
  audio.value.srcObject = stream
  audio.value.play()
  mediaRecorder = new MediaStreamRecorder(stream)
  mediaRecorder.mimeType = "audio/wav" // check this line for audio/wav
  mediaRecorder.ondataavailable = function (blob: Blob) {
    blobs.push(blob)
  }
}

function onMediaError(e: Error) {
  console.error("media error", e)
}

function stop() {
  mediaRecorder?.stop();
}

function record() {
  captureUserMedia({
    audio: true
  }, onMediaSuccess, onMediaError)
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
