<template>
  <div class="app-container">
    <div id="waveform" style="width: 100%" />
    <div id="wave-timeline" />
  </div>
</template>

<script setup lang="ts">
import { useRoute, useRouter } from 'vue-router';
import { reactive, ref, onMounted } from "vue"
import WaveSurfer from "wavesurfer.js"
import timeline from "wavesurfer.js/dist/plugin/wavesurfer.timeline"
import cursor from "wavesurfer.js/dist/plugin/wavesurfer.cursor"
import markers from "wavesurfer.js/dist/plugin/wavesurfer.markers"

onMounted(() => {
  const wavesurfer = WaveSurfer.create({
    container: "#waveform",
    waveColor: "violet",
    progressColor: "purple",
    normalize: true,
    plugins: [
      timeline.create({
        container: "#wave-timeline"
      }),
      cursor.create({
        showTime: true,
        opacity: 1,
        customShowTimeStyle: {
          "background-color": "#000",
          color: "#fff",
          padding: "2px",
          "font-size": "10px"
        }
      })
    ]
  })
  wavesurfer.once("ready", function () {
    wavesurfer.playPause();
    wavesurfer.setVolume(1);
    wavesurfer.setMute(false);
  })
  const route = useRoute()

  wavesurfer.load("/api/wav/raw?id=" + route.params.id)
})
</script>

<style lang="scss" scoped>
#wave-timeline {
  margin-top: 10px;
}
</style>
