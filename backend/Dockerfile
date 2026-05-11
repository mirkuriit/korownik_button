FROM python:3.13-slim

ARG FASTAPI_PORT
ENV FASTAPI_PORT=${FASTAPI_PORT} 
WORKDIR /app

COPY requirements.txt .
RUN pip install -r requirements.txt

COPY . .

CMD uvicorn main:app --host 0.0.0.0 --port "$FASTAPI_PORT"



