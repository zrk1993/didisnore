import { request } from "@/utils/service"

export function getWavPage(data: object) {
  return request({
    url: "wav/page",
    method: "get",
    params: data
  })
}
