#!/usr/bin/python

import sys
import subprocess
import re
import os
import urllib
import errno
import HTMLParser

def Download(path, name = None, auth = None):
	cmd = ['wget', path]
	if auth:
		u, p = auth
		cmd += ['--user=%s' % u, '--password=%s' % p]
	if name:
		cmd += ['-O', name]
	subprocess.check_output(cmd, stderr=subprocess.STDOUT)

class Crawler:
	def __init__(self, auth):
		self._auth = auth
		self._p = re.compile('.*<a href=\"([^\"]+)\".*\n?')
		self._html = HTMLParser.HTMLParser()

	def crawl(self, path, ndir = None):
		if not ndir:
			ndir = next(s for s in path.split('/')[::-1] if s) + '/'
		elif ndir[-1] != '/':
			ndir += '/'
		try:
			os.makedirs(ndir)
		except OSError as e:
			if e.errno != errno.EEXIST:
				raise
		os.chdir(ndir)
		Download(path, auth = self._auth)
		subs = []
		with open('index.html') as f:
			for l in f:
				m = self._p.match(l)
				if m:
					subs.append(self._html.unescape(m.group(1)))
		dirs = [s for s in subs if s and s[0] != '/' and s[-1] == '/']
		files = [s for s in subs if s and s[0] != '/' and s[-1] != '/']
		for s in files:
			n = urllib.unquote(s).decode('utf-8', 'ignore')
			print(path + n)
			Download(path + s, n, self._auth)
		for s in dirs:
			self.crawl(path + s, urllib.unquote(s))
		os.chdir('..')
	
if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("usage: %s path [user]" % sys.argv[0])
	else:
		auth = (sys.argv[2], getpass.getpass('Password: '))
		path = sys.argv[1] 
		c = Crawler(auth)
		c.crawl(path)

