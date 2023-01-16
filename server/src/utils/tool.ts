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
    const int = inputArray[i];
    const float = (int >= 0x8000) ? -(0x10000 - int) / 0x8000 : int / 0x7FFF;
    output[i] = float;
  }
  return output;
}
