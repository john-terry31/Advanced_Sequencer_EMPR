from Tkinter import *
from ttk import *

objectLst = [i for i in range(10)]
patternLst = ['Normal', 'Fade', 'Gradual', 'Flashing']


class NotebookDemo(Frame):

    def __init__(self, isapp=True, name='sequencer'):
        Frame.__init__(self, name=name)
        self.pack(expand=Y, fill=BOTH)
        self.master.title('Advanced Sequencer')
        self.master.geometry('500x300')
        self.isapp = isapp
        self.pSpinArray = []
        self._create_widgets()

    def _create_widgets(self):
        self._create_main_panel()

    def _create_main_panel(self):
        demoPanel = Frame(self, name='main')
        demoPanel.pack(side=TOP, fill=BOTH, expand=Y)

        # create the notebook
        nb = Notebook(demoPanel, name='notebook')

        # extend bindings to top level window allowing
        #   CTRL+TAB - cycles thru tabs
        #   SHIFT+CTRL+TAB - previous tab
        #   ALT+K - select tab using mnemonic (K = underlined letter)
        nb.enable_traversal()

        nb.pack(fill=BOTH, expand=Y, padx=2, pady=3)
        self._create_descrip_tab(nb)
        self._create_disabled_tab(nb)
        self._create_text_tab(nb)
        self.createPacketTab(nb)
        self.createSequenceTab(nb)

    def _create_descrip_tab(self, nb):
        # frame to hold contentx
        frame = Frame(nb, name='descrip')

        # widgets to be displayed on 'Description' tab
        msg = [
            "Ttk is the new Tk themed widget set. One of the widgets ",
            "it includes is the notebook widget, which provides a set ",
            "of tabs that allow the selection of a group of panels, ",
            "each with distinct content. They are a feature of many ",
            "modern user interfaces. Not only can the tabs be selected ",
            "with the mouse, but they can also be switched between ",
            "using Ctrl+Tab when the notebook page heading itself is ",
            "selected. Note that the second tab is disabled, and cannot "
            "be selected."]

        lbl = Label(frame, wraplength='4i', justify=LEFT, anchor=N,
                    text=''.join(msg))
        neatVar = StringVar()
        btn = Button(frame, text='Neat!', underline=0,
                     command=lambda v=neatVar: self._say_neat(v))
        neat = Label(frame, textvariable=neatVar, name='neat')

        # position and set resize behaviour
        lbl.grid(row=0, column=0, columnspan=2, sticky='new', pady=5)
        btn.grid(row=1, column=0, pady=(2, 4))
        neat.grid(row=1, column=1, pady=(2, 4))
        frame.rowconfigure(1, weight=1)
        frame.columnconfigure((0, 1), weight=1, uniform=1)

        # bind for button short-cut key
        # (must be bound to toplevel window)
        self.winfo_toplevel().bind('<Alt-n>', lambda e, v=neatVar: self._say_neat(v))

        # add to notebook (underline = index for short-cut character)
        # nb.add(frame, text='Description', underline=0, padding=2)

    def _say_neat(self, v):
        v.set('Yeah, I know...')
        self.update()
        self.after(500, v.set(''))

    # =============================================================================
    def _create_disabled_tab(self, nb):
        # Populate the second pane. Note that the content doesn't really matter
        frame = Frame(nb)
        # nb.add(frame, text='Disabled', state='disabled')

    # =============================================================================
    def _create_text_tab(self, nb):
        # populate the third frame with a text widget
        frame = Frame(nb)

        txt = Text(frame, wrap=WORD, width=40, height=10)
        vscroll = Scrollbar(frame, orient=VERTICAL, command=txt.yview)
        txt['yscroll'] = vscroll.set
        vscroll.pack(side=RIGHT, fill=Y)
        txt.pack(fill=BOTH, expand=Y)

        # add to notebook (underline = index for short-cut character)
        # nb.add(frame, text='Text Editor', underline=0)

    # =============================================================================
    def createPacketTab(self, nb):
        # Populate the packet frame
        packetFrame = Frame(nb)

        comboContainer = Labelframe(packetFrame)
        comboContainer.pack(pady=5, side=TOP, fill=X)
        pLabel = Label(comboContainer, text="Packet:")
        pLabel.pack(padx=5, side=LEFT)
        pCombo = Combobox(comboContainer, state='readonly', values=objectLst)
        pCombo.pack(side=LEFT)

        numContainer = Labelframe(packetFrame, height=100, text='Packet values:')
        numContainer.pack(side=TOP, fill=X)
        for col in range(10):
            self.pSpinArray.append(Spinbox(numContainer, from_=0, to=9, wrap=TRUE, width=3))
            self.pSpinArray[col].grid(row=0, column=col, padx=5, pady=50)
        numContainer.rowconfigure(0, weight=1)
        for obj in objectLst:
            numContainer.columnconfigure(obj, weight=1, uniform=1)

        butContainer = Labelframe(packetFrame)
        butContainer.pack(side=TOP, fill=BOTH)
        saveB = Button(butContainer, text='Save')
        saveB.grid(row=0, column=0, padx=20, pady=10)
        outSingle = Button(butContainer, text='Save and output')
        outSingle.grid(row=0, column=1, padx=20, pady=10)
        outLoop = Button(butContainer, text='Save and loop')
        outLoop.grid(row=0, column=2, padx=20, pady=10)
        butContainer.rowconfigure(0, weight=1)
        butContainer.columnconfigure((0, 1, 2), weight=1, uniform=1)

        # Add to notebook
        nb.add(packetFrame, text='Packets', padding=2)

    # =============================================================================
    def createSequenceTab(self, nb):
        # Populate the sequence frame
        sequenceFrame = Frame(nb)
        self.createSeqContainer(sequenceFrame)

        self.createExtraSectionContainer(sequenceFrame)

        self.createButtonContainer(sequenceFrame)

        nb.add(sequenceFrame, text='Sequences', padding=2)

    def createSeqContainer(self, tab):
        sContainer = Labelframe(tab)
        sContainer.pack(fill=X, side=TOP)
        sLabel = Label(sContainer, text='Sequence:')
        sLabel.grid(row=0, column=0, padx=5)
        sCombo = Combobox(sContainer, state='readonly', values=objectLst)
        sCombo.grid(row=0, column=1)

        repLabel = Label(sContainer, text='Repeats:')
        repLabel.grid(row=1, column=0, padx=5, pady=5)
        repEntry = Entry(sContainer, width=3)  # Needs to have validation for numeric entry
        repEntry.grid(row=1, column=1, pady=5)
        patLabel = Label(sContainer, text='Pattern:')
        patLabel.grid(row=2, column=0, padx=5)
        patCombo = Combobox(sContainer, state='readonly', values=patternLst)
        patCombo.grid(row=2, column=1)
        clearB = Button(sContainer, text='Clear All')
        clearB.grid(row=2, column=2, padx=75)

    def createExtraSectionContainer(self, tab):
        currentRows = []

        headerContainer = Labelframe(tab)
        headerContainer.pack(fill=X, side=TOP)
        startHeaderLbl = Label(headerContainer, text='Section Start')
        startHeaderLbl.grid(row=0, column=0, padx=5)
        endHeaderLbl = Label(headerContainer, text='Section End')
        endHeaderLbl.grid(row=0, column=1, padx=5)
        repHeaderLbl = Label(headerContainer, text='Repeats')
        repHeaderLbl.grid(row=0, column=2, padx=5)
        patHeaderLbl = Label(headerContainer, text='Pattern')
        patHeaderLbl.grid(row=0, column=3, padx=5)

        self.blankSection(headerContainer, currentRows)

    def blankSection(self, container, rows):
        startBox = Spinbox(container, from_=0, to=9, wrap=TRUE, width=3)
        startBox.grid(row=len(rows)+1, column=0, padx=5)
        endBox = Spinbox(container, from_=0, to=9, wrap=TRUE, width=3)
        endBox.grid(row=len(rows)+1, column=1, padx=5)
        repBox = Entry(container, width=3)
        repBox.grid(row=len(rows)+1, column=2, padx=5)
        patBox = Combobox(container, state='readonly', values=patternLst)
        patBox.grid(row=len(rows)+1, column=3, padx=5)
        addB = Button(container, text='Add')
        addB.grid(row=len(rows)+1, column=4, padx=5)

    def createButtonContainer(self, container):
        butContainer = Labelframe(container)
        butContainer.pack(fill=BOTH, side=TOP)

        saveB = Button(butContainer, text='Save')
        saveB.grid(row=0, column=0, padx=50, pady=10)
        outputB = Button(butContainer, text='Save and Output')
        outputB.grid(row=0, column=1, padx=50, pady=10)
    # def addNewSection(self):



if __name__ == '__main__':
    NotebookDemo().mainloop()
