<template>
  <div @click="onClick">{{ a }}</div>
  <div @click="onClick2">{{ b }}</div>
  <div>{{ state }}</div>
</template>

<script setup lang='ts'>
import { ref, reactive, watch } from 'vue';

const state = reactive({
  a: 1,
  b: '2',
  c: true
})

let a = ref(1)
let b = reactive({ b: { c: 2 } })
let c: any = 2

watch(b, () => {
  c = b
}, { deep: false })

function onClick2() {
  b.b.c += 1
}

function onClick() {
  a.value += 1
}

</script>

<!-- <script lang='ts'>
export default {
  data() {
    return {
      a: 'a'
    }
  },
  methods: {
    onClick() {
      this.a = this.testFun(this.a)
    },
    testFun(a: string) {
      return a + a
    }
  }
}
</script> -->
