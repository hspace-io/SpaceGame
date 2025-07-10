from io import BytesIO
import os

from fastapi import FastAPI, WebSocket
from pymemcache.client.base import Client
import pycurl

FLAG = os.getenv("FLAG", "test_flag")

app = FastAPI(docs_url=None, redoc_url=None)

client = Client("memcached")
client.set("flag", FLAG)


@app.get("/")
def read_root():
    client.set("flag", FLAG)
    return {"Hello": "Proxy"}


@app.get("/get")
def get_value(key: str):
    if "flag" in key.lower():
        return {"status": 403, "response": "Forbidden"}

    return client.get(key)


@app.get("/set")
def set_value(key: str, value: str):
    if "flag" in key.lower():
        return {"status": 403, "response": "Forbidden"}

    return client.set(key, value)


@app.get("/healthcheck")
def healthcheck(check_url: str):
    if not check_url:
        return {"status": 400, "response": "Invalid URL"}

    try:
        print("healthcheck: ", check_url)
        c = pycurl.Curl()
        response_buffer = BytesIO()
        c.setopt(c.URL, check_url)
        c.setopt(c.WRITEDATA, response_buffer)
        c.perform()
        return {
            "status": c.getinfo(pycurl.RESPONSE_CODE),
            "response": response_buffer.getvalue().decode(),
        }
    except Exception as e:
        print(e)
        return {"status": 500, "response": "Error"}


@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_text()
        await websocket.send_text(f"Message text was: {data}")
