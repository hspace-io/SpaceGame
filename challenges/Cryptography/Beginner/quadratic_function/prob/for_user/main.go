package main

import (
  "fmt"
)

func twoencrypt() {
  ra := 153
  rb := 592
  rc := 104

  var strings string
  fmt.Scanln(&strings)

  output := ""
  a := ra + rb
  b := rb + rc
  c := rc + ra

  for _, char := range strings {
    x := int(char)
    fy := ((a * b * c) * (a * b * c))
    a = (fy % (a + b) + a) % (b + c) + 1
    b = (b % (b + c + a) + b) % (c + a) + 1
    c = (fy % (c + a) + c) % (a + b) + (a * b * c) % 3
    y := a * ((x + b) * (x + b)) + c
    output += fmt.Sprintf("%d ", y)
  }
  fmt.Println(output)
}

func main() {
  twoencrypt()
}
