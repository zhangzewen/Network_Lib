#!/bin/bash

#curl -v -H'Content-Type: application/json' 'http://192.168.1.154/haiyun/a.gif' -d '
curl -v -H'Content-Type: application/json' 'http://localhost:8080/haiyun/a.gif' -d '
{
    "dsp_id": "369",
    "token": "8409a4022a2c4c5a",
    "version": "",
    "id": "74ccf2af-6488-49cc-ab65-bea093b514ac203.93.9.051013691429690107187",
    "imp": [
        {
            "id": "74ccf2af-6488-49cc-ab65-bea093b514ac51011429690107187",
            "tagid": 5101,
            "bidfloor": 250,
            "banner": {
                "w": 300,
                "h": 250
            }
        }
    ],
    "site": {
        "name": "黑龙小说推广300x250",
        "page": "http%3A%2F%2Fwww.haiyunx.cn%2Ftest1.html",
        "ref": ""
    },
    "device": {
        "ua": "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:37.0) Gecko/20100101 Firefox/37.0",
        "ip": "203.93.9.0"
    },
    "user": {
        "id": "74ccf2af-6488-49cc-ab65-bea093b514ac"
    }
}
'
