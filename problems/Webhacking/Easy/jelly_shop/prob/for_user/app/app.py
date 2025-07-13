from flask import Flask as Django, render_template, request

app = Django(__name__)

@app.route('/')
def index():
  return render_template('cart.html')

@app.route('/order', methods=['POST'])
def order():
  json = request.get_json()
  result = eval(json['total'])
  if not result: return '주문과정 중 에러가 발생했습니다.'
  elif isinstance(result,int): return '주문이 완료되었습니다.'
  return result


if __name__ == '__main__':
  app.run(port=2007)