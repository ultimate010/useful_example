#-*- coding: utf-8 -*-


import requests
import sys


POST_URL = 'http://www.newsmth.net/nForum/article/CouponsLife/ajax_post.json'
LOGIN_URL = 'http://www.newsmth.net/nForum/user/ajax_login.json'

TIMEOUT = 10



def send(title,msg,cookie):
    post_data = {
            "subject":title,
            "content":msg,
            "signature":0,
            #"auth":0,
            "id":0
    }
    post_header = {
            "Host":"www.newsmth.net",
            "Connection":"keep-alive",
            "Content-Length":"152",
            "Cache-Control":"no-cache",
            "Pragma":"no-cache",
            "Origin":"http://www.newsmth.net",
            "X-Requested-With":"XMLHttpRequest",
            "Content-Type":"application/x-www-form-urlencoded; charset=UTF-8",
            "Accept":"*/*",
            "User-Agent":"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.117 Safari/537.36",
            "DNT":"1",
            "Referer":"http://www.newsmth.net/nForum/",
            "Accept-Language":"zh-CN,zh;q=0.8,en;q=0.6,zh-TW;q=0.4",
            "Cookie": cookie
    }
    try:
        req = requests.post(POST_URL , data=post_data, headers=post_header,  timeout=TIMEOUT )
        print req.text
    except Exception as e:
        print e
def getCookie(username,password):
    post_data = {
            'id' : username,
            'passwd' : password
    }
    post_header = {
            'Host':'www.newsmth.net',
            'Connection':'keep-alive',
            'Content-Length':'24',
            'Cache-Control':'no-cache',
            'Pragma':'no-cache',
            'Origin':'http://www.newsmth.net',
            'X-Requested-With':'XMLHttpRequest',
            'Content-Type':'application/x-www-form-urlencoded; charset=UTF-8',
            'Accept':'application/json, text/javascript, */*; q=0.01',
            'User-Agent':'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.117 Safari/537.36',
            'DNT':'1',
            'Referer':'http://www.newsmth.net/nForum/',
            'Accept-Encoding':'gzip,deflate,sdch',
            'Accept-Language':'zh-CN,zh;q=0.8,en;q=0.6,zh-TW;q=0.4',
            'Cookie':"left-index=00000000000; nforum-left=01000; NFORUM=a79m69a75679q4bgs4otjqmv94; tmc=1.88525828.49278139.1395717709300.1395717709300.1395717709300; tma=88525828.12563461.1395648387816.1395648387816.1395717709308.2; tmd=22.88525828.12563461.1395648387816.; bfd_session_id=bfd_g=9b7dd4ae5263d4330009a87e000bdc7a521d5b2b&bfd_s=88525828.6131978.1395717709294; main[XWJOKE]=hoho; main[UTMPUSERID]=guest; main[UTMPKEY]=56610065; main[UTMPNUM]=23050; channel-index=100000000",
    }
    try:
        req = requests.post(LOGIN_URL , data=post_data, headers=post_header,  timeout=TIMEOUT)
        return req.headers['set-cookie']
    except Exception as e:
        print e


def main():
    if len(sys.argv) < 3:
        print "usage : %s title content" %sys.argv[0]
        exit(1)
    cookstr = '';
    cookie = getCookie('','').split(',')
    for co in cookie[3 : ]:
        cookstr += co.split(' ')[1]
    print cookstr

    send(sys.argv[1],sys.argv[2],cookstr)



if __name__ == "__main__":
    main()

