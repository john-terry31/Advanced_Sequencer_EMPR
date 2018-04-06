from Tkinter import *
from ttk import *
import AdvancedSequencer as AdvSeq
import sequencer as Seq

_sequenceLength = 10
_packetLength = 10
_maxSeqRepeats = 99
_objectLst = [i for i in range(10)]
_patternLst = ['Normal', 'Fade', 'Gradual', 'Flashing']


class NotebookDemo(Frame):

    def __init__(self, isapp=True, name='sequencer'):
        Frame.__init__(self, name=name)
        self.pack(expand=Y, fill=BOTH)
        self.master.title('Advanced Sequencer')
        self.master.geometry('500x300')
        self.isapp = isapp

        self.pSpinArray = []  # Holds the packet spinbox instances
        self.pCombo = None  # Packet tab combobox
        self.pComboValue = None
        self.packet = None
        self.sSpinArray = [] # Holds the sequence spinbox instances

        # Sequence tab variables
        self.sequenceFrame = None
        self.sContainer = None
        self.sLabel = None
        self.sCombo = None
        self.sSpinContainer = None
        self.repLabel = None
        self.repInput = None
        self.patLabel = None
        self.patCombo = None
        self.clearB = None

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
        pVar = StringVar()
        self.pCombo = Combobox(comboContainer, state='readonly', values=_objectLst, textvariable=pVar)
        self.pCombo.pack(side=LEFT)
        self.pCombo.bind('<<ComboboxSelected>>', lambda e, p=pVar: self.comboCallback(p))

        numContainer = Labelframe(packetFrame, height=100, text='Packet values:')
        numContainer.pack(side=TOP, fill=X)
        for col in range(_packetLength):
            self.pSpinArray.append(Spinbox(numContainer, from_=0, to=AdvSeq.maxPacketVal, wrap=TRUE, width=3))
            self.pSpinArray[col].grid(row=0, column=col, padx=5, pady=50)
        numContainer.rowconfigure(0, weight=1)
        for col in range(_packetLength):
            numContainer.columnconfigure(col, weight=1, uniform=1)

        butContainer = Labelframe(packetFrame)
        butContainer.pack(side=TOP, fill=BOTH)
        saveB = Button(butContainer, text='Save', command=self.saveButton)
        saveB.grid(row=0, column=0, padx=20, pady=10)
        outSingle = Button(butContainer, text='Save and output', command=self.saveOutputPacketButton)
        outSingle.grid(row=0, column=1, padx=20, pady=10)
        outLoop = Button(butContainer, text='Save and loop', command=self.saveLoopPacketButton)
        outLoop.grid(row=0, column=2, padx=20, pady=10)
        butContainer.rowconfigure(0, weight=1)
        butContainer.columnconfigure((0, 1, 2), weight=1, uniform=1)

        # Add to notebook
        nb.add(packetFrame, text='Packets', padding=2)

    def comboCallback(self, combo):
        self.pComboValue = self.pCombo.get()
        if not self.pCombo.get() == '':
            for obj in _objectLst:
                self.pSpinArray[obj].delete(0, 3)
                self.pSpinArray[obj].insert(0, AdvSeq.packets[int(self.pComboValue)][obj])

    def getCurPacket(self):
        return self.pCombo.get()

    def updatePacket(self, pNo):
        for slot in range(_packetLength):
            AdvSeq.packets[int(pNo)][slot] = self.pSpinArray[slot].get()
        print("Changed")

    def saveButton(self):
        if self.getCurPacket() != '':
            self.updatePacket(self.getCurPacket())
            Seq.savePacket(packetNo=self.getCurPacket(), values=AdvSeq.packets[int(self.getCurPacket())])

    def saveOutputPacketButton(self):
        if self.getCurPacket() != '':
            self.updatePacket(self.getCurPacket())
            Seq.sendPacket(packetNo=self.pCombo.get(), newValues=AdvSeq.packets[int(self.getCurPacket())])

    def saveLoopPacketButton(self):
        if self.getCurPacket() != '':
            self.updatePacket(self.getCurPacket())
            Seq.sendPacketLoop(packetNo=self.pCombo.get(), newValues=AdvSeq.packets[int(self.getCurPacket())])

    # =============================================================================
    def createSequenceTab(self, nb):
        # Populate the sequence frame
        self.sequenceFrame = Frame(nb)
        self.createSeqContainer(self.sequenceFrame)
        self.createExtraSectionContainer(self.sequenceFrame)
        self.createButtonContainer(self.sequenceFrame)
        nb.add(self.sequenceFrame, text='Sequences', padding=2)

    def createSeqContainer(self, tab):
        self.sContainer = Labelframe(tab)
        self.sContainer.pack(fill=X, side=TOP)
        self.sLabel = Label(self.sContainer, text='Sequence:')
        self.sLabel.grid(row=0, column=0)
        self.sCombo = Combobox(self.sContainer, state='readonly', values=_objectLst)
        self.sCombo.grid(row=0, column=1, padx=10)
        # Add sequence spinbox
        self.sSpinContainer = Labelframe(self.sContainer)
        self.sSpinContainer.grid(row=0, column=2, columnspan=10)
        for col in range(_packetLength):
            self.sSpinArray.append(Spinbox(self.sSpinContainer, from_=0, to=len(_objectLst), wrap=TRUE, width=3))
            self.sSpinArray[col].pack(side=LEFT)
        for col in range(_packetLength+2):
            if col <= 2:
                self.sContainer.columnconfigure(col, weight=2, uniform=1)
            else:
                self.sContainer.columnconfigure(col, weight=1, uniform=1)

        self.repLabel = Label(self.sContainer, text='Repeats:')
        self.repLabel.grid(row=1, column=0, padx=5, pady=5)
        self.repInput = Spinbox(self.sContainer, from_=0, to=_maxSeqRepeats, wrap=TRUE,
                                width=3)    # #################### Needs to have validation for numeric entry
        self.repInput.grid(row=1, column=1, pady=5)
        self.patLabel = Label(self.sContainer, text='Pattern:')
        self.patLabel.grid(row=2, column=0, padx=5)
        self.patCombo = Combobox(self.sContainer, state='readonly', values=_patternLst)
        self.patCombo.grid(row=2, column=1)
        self.clearB = Button(self.sContainer, text='Clear All')
        self.clearB.grid(row=2, column=2, padx=75, columnspan=6)

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
        endBox = Spinbox(container, from_=0, to=_maxSeqRepeats, wrap=TRUE, width=3)
        endBox.grid(row=len(rows)+1, column=1, padx=5)
        repBox = Spinbox(container, from_=0, to=9, wrap=TRUE, width=3)
        repBox.grid(row=len(rows)+1, column=2, padx=5)
        patBox = Combobox(container, state='readonly', values=_patternLst)
        patBox.grid(row=len(rows)+1, column=3, padx=5)
        addB = Button(container, text='Add')
        addB.grid(row=len(rows)+1, column=4, padx=5)
        rows.append([None, None, None, None])

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
