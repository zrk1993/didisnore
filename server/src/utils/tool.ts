export function floatTo16Bit(inputArray: [any]) {
  const output = new Int16Array(inputArray.length);
  for (let i = 0; i < inputArray.length; i++) {
    const s = Math.max(-1, Math.min(1, inputArray[i]));
    output[i] = s < 0 ? s * 0x8000 : s * 0x7FFF;
  }
  return output;
}

export function int16ToFloat32(inputArray: [any]) {
  const output = new Float32Array(2048);
  for (var i = 0; i < 2048; i++) {
    const s = inputArray[i];
    const f = (s >= 0x8000) ? -(0x10000 - s) / 0x8000 : s / 0x7FFF;
    output[i] = f;
  }
  return output;
}

export function average(arr: [number]) {
  const sum = arr.reduce((p, i) => p + i, 0)
  return sum / arr.length
}
