# -*- coding: utf-8 -*-

import smtplib
import email.MIMEMultipart
import email.MIMEText
import email.MIMEBase
from email.header import Header
import os.path

def sendMail(server,user,password,From,To,subject,body,file_list):

	server = smtplib.SMTP(server)
	server.docmd("EHLO server")
	server.starttls()
	server.login(user, password)

	main_msg = email.MIMEMultipart.MIMEMultipart()

	#text_msg = email.MIMEText.MIMEText(body)
	text_msg = email.MIMEText.MIMEText(body,'plain','gbk')

	main_msg.attach(text_msg)


	contype = 'application/octet-stream'
	maintype, subtype = contype.split('/', 1)

	for file_name in file_list:
		try:
			data = open(file_name, 'rb')
			file_msg = email.MIMEBase.MIMEBase(maintype, subtype)
			file_msg.set_payload(data.read( ))
			data.close( )
			email.Encoders.encode_base64(file_msg)

			basename = os.path.basename(file_name)
			file_msg.add_header('Content-Disposition','attachment', filename = basename)
			main_msg.attach(file_msg)
		except:
			pass


	main_msg['From'] = From
	main_msg['To'] =To
	main_msg['Subject'] = Header(subject,'gbk')
	main_msg['Date'] = email.Utils.formatdate( )

	fullText = main_msg.as_string( )

	# 用smtp发送邮件
	try:
		server.sendmail(From, To, fullText)
	finally:
		server.quit()

def sendMail_HTML(server,user,password,From,To,subject,body,file_list,reply_to,encode,bLogin=False):

	server = smtplib.SMTP(server)
	server.docmd("EHLO server")

	if bLogin:
		server.starttls()
		server.login(user, password)

	main_msg = email.MIMEMultipart.MIMEMultipart()

	#text_msg = email.MIMEText.MIMEText(body)
	text_msg = email.MIMEText.MIMEText(body,'html',encode)

	main_msg.attach(text_msg)


	contype = 'application/octet-stream'
	maintype, subtype = contype.split('/', 1)

	for file_name in file_list:
		try:
			data = open(file_name, 'rb')
			file_msg = email.MIMEBase.MIMEBase(maintype, subtype)
			file_msg.set_payload(data.read( ))
			data.close( )
			email.Encoders.encode_base64(file_msg)

			basename = os.path.basename(file_name)
			file_msg.add_header('Content-Disposition','attachment', filename = basename)
			main_msg.attach(file_msg)
		except:
			pass

	frmHeader = Header ('server status report',encode)
	frmHeader.append('<' + From + '>', 'ascii' )
	main_msg['From'] = frmHeader
	main_msg['To'] =To
	main_msg['Reply-To'] = reply_to
	main_msg['Subject'] = Header(subject,encode)
	main_msg['Date'] = email.Utils.formatdate( )

	fullText = main_msg.as_string( )

	# 用smtp发送邮件
	try:
		result = server.sendmail(From, To, fullText)
		return result
	finally:
		server.quit()

	return None


if __name__=="__main__":

	data = ''

	sendMail_HTML ( 'smtp.126.com', 'newsalertservice','', 'newsalertservice@126.com', "", "server reboot", 'vultr server reboot', (),'no-reply@ultimate010.com','utf8',True)
