import { request } from "@/utils/service"

/** 获取登录验证码 */
export function getWavRaw() {
  return request({
    url: "wav/get",
    method: "get"
  })
}
