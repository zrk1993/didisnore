<template>
  <div class="app-container">
    <div id="waveform" style="width: 100%" />
    <div id="wave-timeline" />
  </div>
</template>

<script setup lang="ts">
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
      }),
      markers.create({
        markers: [
          {
            time: 0,
            label: "BEGIN",
            color: "#ff990a",
            preventContextMenu: true
          },
          {
            time: 5.5,
            label: "V1",
            color: "#ff990a",
            draggable: true
          },
          {
            time: 24,
            label: "END",
            color: "#00ffcc",
            position: "top"
          }
        ]
      })
    ]
  })
  wavesurfer.once("ready", function () {
    wavesurfer.playPause();
    wavesurfer.setVolume(1);
    wavesurfer.setMute(false);
  })
  wavesurfer.load("/api/wav/raw?id=230211_100225_46")
})
</script>

<style lang="scss" scoped>
#wave-timeline {
  margin-top: 10px;
}
</style>
