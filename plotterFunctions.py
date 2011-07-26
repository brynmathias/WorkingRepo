#!/usr/bin/env python


import ROOT as r
import math
import os, commands, sys




# Plan:
# Redesign Auto-Plotter so that its not such a mess.
# Design a plotter class that takes a list of inputs N long
# Give scaling options by class.scale(scale)
# Have a scalable option defaults to true, but for data is false so class.scale()
# On a member which is data like returns the original object.
# 
# Have class.SetColours(list) - set list of colours for histograms in order.
# Options to draw all from sub dirs or draw from list
# plots = sweetPlots(someList)
# plots.Draw(List) 
# or plots.Draw("All")
# 
# Deffinately needs a plots.makeTable()
# 
# 

class Hist(object):
	"""docstring for Hist"""
	def __init__(self, arg):
		super(Hist, self).__init__()
		self.arg = arg
  def SetLegend(self):
  	"""docstring for SetLegend"""
  	pass





class sweetPlots(object):
	"""docstring for sweetPlots"""
	def __init__(self, arg):
		super(sweetPlots, self).__init__()
		self.arg = arg
		