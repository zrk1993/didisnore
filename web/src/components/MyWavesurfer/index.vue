<template>
  <div ref="waveform" class="waveform" style="width: 100%"></div>
</template>

<script setup lang="ts">
import WaveSurfer from "wavesurfer.js"
import timeline from "wavesurfer.js/dist/plugin/wavesurfer.timeline"
import cursor from "wavesurfer.js/dist/plugin/wavesurfer.cursor"
import markers from "wavesurfer.js/dist/plugin/wavesurfer.markers"
import { onMounted, onUnmounted, ref, defineProps, watch } from "vue"


const props = defineProps(["url"])

let wavesurfer: WaveSurfer
const waveform = ref<any>(null)

watch(props.url, () => {
  wavesurfer?.load(props.url)
})

onMounted(() => {
  wavesurfer = WaveSurfer.create({
    container: waveform.value,
    waveColor: "violet",
    progressColor: "purple"
  })
  wavesurfer.load(props.url)
})

</script>

<style lang="scss" scoped>
.waveform {
  min-height: 128px;
}
</style>
