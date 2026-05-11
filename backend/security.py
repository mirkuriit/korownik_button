import os

from dotenv import load_dotenv
from fastapi.security import APIKeyHeader
from fastapi import Depends

load_dotenv()

API_KEY = os.getenv("X-API-KEY")
security_header_cheme = APIKeyHeader(name="X-API-KEY")


def verify_key_depend(key: str = Depends(security_header_cheme)):
    if key != API_KEY:
        raise HTTPException(status_code=401, detail="Invalid API key")
