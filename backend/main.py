from fastapi import FastAPI, APIRouter, Depends
from pydantic import BaseModel
from typing import Literal

from security import verify_key_depend

app = FastAPI()
router = APIRouter(
    prefix="/koworking-api",
    dependencies=[Depends(verify_key_depend)]
)

class Korownik:
    is_open = True

    def get_status(self):
        return self.is_open

    def change_status(self):
        self.is_open = not self.is_open


class KorownikStatus(BaseModel):
    status: bool


korownik = Korownik()    

@router.post("/")
async def change_korownik_status() -> KorownikStatus:
    korownik.change_status()
    return KorownikStatus(status=korownik.get_status())


@router.get("/") 
async def get_korownik_status() -> KorownikStatus:
    return KorownikStatus(status=korownik.get_status())

app.include_router(router)
