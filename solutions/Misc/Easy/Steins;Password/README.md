# Steins;Password

# Usage

-

# Concept

- Image Channel

# Writeup

주어진 prob.png를 각각 R 채널, G 채널, B 채널만 남기고 보았을 경우
각각이 유효한 QR 코드를 가진다는 것을 알 수 있습니다.

R, G, B 채널에서 얻은 QR 코드의 값을 순서대로 모두 이어 붙이면 하나의 Base64 문자열이 됩니다.

해당 Base64 문자열을 decode하면 플래그를 얻을 수 있습니다.
