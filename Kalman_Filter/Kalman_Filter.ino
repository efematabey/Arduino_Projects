struct Kalman { // Kalman filtresi için gereken değişkenler
  float Eest; // Tahmin edilen değerin hatası
  float Emea; // Ölçümdeki hata payı
  float Value; // Okunan analog veri
  float CurrEst; // Şuanki tahmin edilen değer
  float PrevEst; // Önceki tahmin
  float KG; // Kalman kazancı
};
struct Kalman K;

const float Q = 0.01; // Sabit bir değer olan "Q" (Process Variance) maximum 1 olabilir.

void setup() {
  pinMode(A6, INPUT);
  Serial.begin(9600);

  // Kalman filtresinin başlangıç değerleri
  K.Eest = 2; // Tahmin edilen değerin hatası
  K.Emea = 3; // Ölçümdeki hata payı (Değişmez sabittir)
  K.PrevEst = 50; // Önceki tahmin
}

void loop() {
  KalmanFilter();
  Serial.print("Ölçüm:");
  Serial.print(K.Value);
  Serial.print(" Kalman:");
  Serial.println(K.CurrEst); // Serial Plotter ekranına "Kalman" adıyla Kalman Filtresinden geçmiş sinyal çizdirilir.
}

void KalmanFilter() {
  K.Value = analogRead(A6); 
  K.KG = (K.Eest / (K.Eest + K.Emea)); // Kalman kazancı(KG) hesaplanır, bu değer 0 ile 1 arasındadır.
  K.CurrEst = K.PrevEst + K.KG * (K.Value - K.PrevEst); // Şuanki Tahmin edilen değer hesaplanır.
  K.Eest = (1 - K.KG) * K.Eest + fabsf(K.PrevEst - K.CurrEst) * Q; // Tahmin edilen değerin hatası hesaplanır.
  K.PrevEst = K.CurrEst; // Önceki tahmin, şuanki tahmin edilen değere eşitlenir.
}