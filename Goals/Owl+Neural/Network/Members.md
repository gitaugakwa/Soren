### m_Title
type: std::string
The name of the network.

---
### m_Locked
type: bool
Determines if the network will be modified.

---
### m_EventCallback
type: [[EventCallbackFn]]
Function called when an [[Event]] occurs.

---
### m_Layers
type: std::vector<[[Layer]]>
Holds all [[Layer|Layers]] currently owned by [[Network]].

---
### m_Status
type: [[NetworkStatus]]
Holds the status of the current [[Network]].

---
### m_PutFunc
type: std::function<void([[Layer]]& layer)>
The function called when putting data into a [[Layer]].

---
### m_PullFunc
type: std::function<void(const [[Layer]]& layer)>
The function called when pulling data from a [[Layer]].

---
### m_FileInputs
type: std::vector<[[FileInput]]<Input_t>>
Holds all [[FileInput|FileInputs]] currently owned by [[Network]].
Would be best if it were a template.

---
### m_FileOutputs
type: std::vector<[[FileOutput]]<Output_t>>
Holds all [[FileOutput|FileOutputs]] currently owned by [[Network]].
Would be best if it were a template.

---
### m_BaseDir
type: std::filesystem::path
Holds the base directory of the network.

---
### m_Callbacks
type: [[NetworkCallbacks]]
Holds callbacks for all [[Event|Events]].

---
### m_Events
type: [[NetworkEvents]]
Holds all [[Event|Events]] that occur after the main [[Event]] occurs. 
