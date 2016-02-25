#!/usr/bin/env python

import ROOT
import os

class plot_saver:
    i = 0
    
    def __init__(self, plot_dir=None, html=True, log=True, root=True, pdf=False, pdf_log=False, C=False, C_log=False, size=(820,630)):
        self.c = ROOT.TCanvas('c%i' % plot_saver.i, '', *size)
        plot_saver.i += 1
        self.saved = []
        self.html = html
        self.set_plot_dir(plot_dir)
        self.log = log
        self.root = root
        self.pdf = pdf
        self.pdf_log = pdf_log
        self.C = C
        self.C_log = C_log

    def __del__(self):
        self.write_index()

    def anchor_name(self, fn):
        return fn.replace('.', '_').replace('/', '_')
    
    def write_index(self):
        if not self.saved or not self.html:
            return
        html = open(os.path.join(self.plot_dir, 'index.html'), 'wt')
        html.write('<html><body><pre>\n')
        html.write('<a href="..">.. (parent directory)</a>\n')
        for i, save in enumerate(self.saved):
            if type(save) == str:
                # this is just a directory link
                html.write('<a href="%s">%10i%32s%s</a>\n' % (save, i, 'change directory: ', save))
                continue

            fn, log, root, pdf, pdf_log, C, C_log = save

            bn = os.path.basename(fn)
            html.write('<a href="#%s">%10i</a> ' % (self.anchor_name(fn), i))
            if log:
                html.write(' <a href="%s">log</a>' % os.path.basename(log))
            else:
                html.write('    ')
            if root:
                html.write(' <a href="%s">root</a>' % os.path.basename(root))
            else:
                html.write('     ')
            if pdf:
                html.write(' <a href="%s">pdf</a>' % os.path.basename(pdf))
            else:
                html.write('     ')
            if pdf_log:
                html.write(' <a href="%s">pdf_log</a>' % os.path.basename(pdf_log))
            else:
                html.write('     ')
            if C:
                html.write(' <a href="%s">C</a>' % os.path.basename(C))
            else:
                html.write('     ')
            if C_log:
                html.write(' <a href="%s">C_log</a>' % os.path.basename(C_log))
            else:
                html.write('     ')
            html.write('  <a href="%s">%s</a>' % (bn, bn))
            html.write('\n')
        html.write('<br><br>')
        for i, save in enumerate(self.saved):
            if type(save) == str:
                continue # skip dir entries
            fn, log, root, pdf, pdf_log, C, C_log = save
            bn = os.path.basename(fn)
            html.write('<h4 id="%s">%s</h4><br>\n' % (self.anchor_name(fn), bn.replace('.png', '')))
            if log:
                html.write('<img src="%s"><img src="%s"><br><br>\n' % (bn, os.path.basename(log)))
            else:
                html.write('<img src="%s"><br><br>\n' % bn)
        html.write('</pre></body></html>\n')
        
    def set_plot_dir(self, plot_dir):
        self.write_index()
        self.saved = []
        if plot_dir is not None and '~' in plot_dir:
            plot_dir = os.path.expanduser(plot_dir)
        self.plot_dir = plot_dir
        if plot_dir is not None:
            os.system('mkdir -p %s' % self.plot_dir)

    def save_dir(self, n):
        if self.plot_dir is None:
            raise ValueError('save_dir called before plot_dir set!')
        self.saved.append(n)

    def save(self, n, log=None, root=None, pdf=None, pdf_log=None, C=None, C_log=None):
        log = self.log if log is None else log
        root = self.root if root is None else root
        pdf = self.pdf if pdf is None else pdf
        pdf_log = self.pdf_log if pdf_log is None else pdf_log
        C = self.C if C is None else C
        C_log = self.C_log if C_log is None else C_log
        
        if self.plot_dir is None:
            raise ValueError('save called before plot_dir set!')
        self.c.SetLogy(0)
        fn = os.path.join(self.plot_dir, n + '.png')
        self.c.SaveAs(fn)
        if root:
            root = os.path.join(self.plot_dir, n + '.root')
            self.c.SaveAs(root)
        if log:
            self.c.SetLogy(1)
            log = os.path.join(self.plot_dir, n + '.log.png')
            self.c.SaveAs(log)
            self.c.SetLogy(0)
        if pdf:
            pdf = os.path.join(self.plot_dir, n + '.pdf')
            self.c.SaveAs(pdf)
        if pdf_log:
            self.c.SetLogy(1)
            pdf_log = os.path.join(self.plot_dir, n + '.log.pdf')
            self.c.SaveAs(pdf_log)
            self.c.SetLogy(0)
        if C:
            C = os.path.join(self.plot_dir, n + '.C')
            self.c.SaveAs(C_fn)
        if C_log:
            self.c.SetLogy(1)
            C_log = os.path.join(self.plot_dir, n + '.log.C')
            self.c.SaveAs(C_log)
            self.c.SetLogy(0)
        self.saved.append((fn, log, root, pdf, pdf_log, C, C_log))

def set_zp2mu_style(date_pages=False):
    ROOT.gROOT.SetStyle('Plain')
    ROOT.gStyle.SetFillColor(0)
    if date_pages:
        ROOT.gStyle.SetOptDate()
    ROOT.gStyle.SetOptStat(111111)
    ROOT.gStyle.SetOptFit(1111)
    ROOT.gStyle.SetPadTickX(1)
    ROOT.gStyle.SetPadTickY(1)
    ROOT.gStyle.SetMarkerSize(.1)
    ROOT.gStyle.SetMarkerStyle(8)
    ROOT.gStyle.SetGridStyle(3)
    ROOT.gStyle.SetPaperSize(ROOT.TStyle.kA4)
    ROOT.gStyle.SetStatW(0.25)
    ROOT.gStyle.SetStatFormat('6.4g')
    ROOT.gStyle.SetPalette(1)
    #ROOT.gStyle.SetTitleFont(52, 'XY')
    #ROOT.gStyle.SetLabelFont(52, 'XY')
    #ROOT.gStyle.SetStatFont(52)
    ROOT.gErrorIgnoreLevel = 1001 # Suppress TCanvas::SaveAs messages.
