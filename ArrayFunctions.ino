// Compute the mean of the array of floats
float GetArrayMean (float *pInArray, int InLength) {
  float AveLaser   = 0;
  float iCount = 0.0;
  if (InLength == 0 ) {
    return 0.00;
  }
  for(int i = 0; i < InLength; i++) {
    if (pInArray[i] >0) { 
      AveLaser = AveLaser + pInArray[i];
      iCount = iCount + 1.0;
    }
  }
  if (iCount == 0.0) {
    return 0.0;
  }
  return AveLaser/iCount;
}

// Find the MAXIMUM of the array of floats
// maxSpectrum = GetArrayMax(resultArray,lengthSpectrum);
float GetArrayMax (float *pInArray, int InLength) {
  float MaxLaser   = 0;
  for(int i = 0; i < InLength; i++) {
    if (pInArray[i]>MaxLaser) {
      MaxLaser = pInArray[i];
    }
  }
  return MaxLaser;
}
// Find the MINIMUM of the array of floats
float GetArrayMin (float *pInArray, int InLength) {
  float MinLaser   = pInArray[0];
  for(int i = 0; i < InLength; i++) {
    if (pInArray[i]<MinLaser) {
        MinLaser = pInArray[i];
    }
  }
  return MinLaser;
}
