from fastapi import FastAPI, Request, Form, Response
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates

from typing import Annotated

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")

templates = Jinja2Templates(directory="templates")

_station_list = [
    "http://link1",
    "http://link2",
    "http://link3"
]


@app.get("/")
def index(request: Request):
    return templates.TemplateResponse(
        request=request, name="index.html", context={}
    )


@app.get("/station_list")
def station_list_html(request: Request):
    return templates.TemplateResponse(
        request=request,
        name="station_list.html",
        context={"url_list": _station_list}
    )


@app.delete("/station_list")
def delete_station(request: Request, id: int):
    _station_list.pop(id-1)
    return templates.TemplateResponse(
            request=request,
            name="station_list.html",
            context={"url_list": _station_list},
            headers={"HX-Location": '{"path":"/station_list", "target":"#station_table"}'}
        )


@app.post("/station_list")
def add_station(request: Request, url: Annotated[str, Form()]):
    _station_list.append(url)
    return templates.TemplateResponse(
            request=request,
            name="station_list.html",
            context={"url_list": _station_list},
            headers={"HX-Location": '{"path":"/station_list", "target":"#station_table"}'}
        )


@app.put("/station_list")
def update_station(request: Request, response: Response, id: int, url: Annotated[str, Form()]):
    _station_list[id-1] = url
    response.headers["HX-Location"] = '{"path":"/station_list", "target":"#station_table"}'
#     return templates.TemplateResponse(
#             request=request,
#             name="station_row.html",
#             context={"count": id, "item": _station_list[id-1]}
#         )


@app.get("/url_edit")
def edit_template(request: Request, id: int):
    return templates.TemplateResponse(
            request=request,
            name="row_edit.html",
            context={"id": id, "url": _station_list[id-1]},
            # headers={"HX-Location": '{"path":"/station_list", "target":"#station_table"}'}
        )