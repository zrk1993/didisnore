declare module "*.vue" {
  import { DefineComponent } from "vue"
  const component: DefineComponent<{}, {}, any>
  export default component
}

declare module "*.scss" {
  const scss: Record<string, string>
  export default scss
}

declare module "wavesurfer.js/dist/plugin/wavesurfer.timeline"
declare module "wavesurfer.js/dist/plugin/wavesurfer.cursor"
declare module "wavesurfer.js/dist/plugin/wavesurfer.markers"
