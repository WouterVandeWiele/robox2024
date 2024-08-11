const connect = async button => {
  import('./install-dialog-D08QNx_L.js');
  let port;
  try {
    port = await navigator.serial.requestPort();
  } catch (err) {
    if (err.name === "NotFoundError") {
      import('./index-D_k7Pxth.js').then(mod => mod.openNoPortPickedDialog(() => connect(button)));
      return;
    }
    alert(`Error: ${err.message}`);
    return;
  }
  if (!port) {
    return;
  }
  try {
    await port.open({
      baudRate: 115200
    });
  } catch (err) {
    alert(err.message);
    return;
  }
  const el = document.createElement("ewt-install-dialog");
  el.port = port;
  el.manifestPath = button.manifest || button.getAttribute("manifest");
  el.overrides = button.overrides;
  el.addEventListener("closed", () => {
    port.close();
  }, {
    once: true
  });
  document.body.appendChild(el);
};

class InstallButton extends HTMLElement {
  connectedCallback() {
    if (this.renderRoot) {
      return;
    }
    this.renderRoot = this.attachShadow({
      mode: "open"
    });
    if (!InstallButton.isSupported || !InstallButton.isAllowed) {
      this.toggleAttribute("install-unsupported", true);
      this.renderRoot.innerHTML = !InstallButton.isAllowed ? "<slot name='not-allowed'>You can only install ESP devices on HTTPS websites or on the localhost.</slot>" : "<slot name='unsupported'>Your browser does not support installing things on ESP devices. Use Google Chrome or Microsoft Edge.</slot>";
      return;
    }
    this.toggleAttribute("install-supported", true);
    const slot = document.createElement("slot");
    slot.addEventListener("click", async ev => {
      ev.preventDefault();
      connect(this);
    });
    slot.name = "activate";
    const button = document.createElement("button");
    button.innerText = "Connect";
    slot.append(button);
    if ("adoptedStyleSheets" in Document.prototype && "replaceSync" in CSSStyleSheet.prototype) {
      const sheet = new CSSStyleSheet();
      sheet.replaceSync(InstallButton.style);
      this.renderRoot.adoptedStyleSheets = [sheet];
    } else {
      const styleSheet = document.createElement("style");
      styleSheet.innerText = InstallButton.style;
      this.renderRoot.append(styleSheet);
    }
    this.renderRoot.append(slot);
  }
}
InstallButton.isSupported = "serial" in navigator;
InstallButton.isAllowed = window.isSecureContext;
InstallButton.style = `
  button {
    position: relative;
    cursor: pointer;
    font-size: 14px;
    font-weight: 500;
    padding: 10px 24px;
    color: var(--esp-tools-button-text-color, #fff);
    background-color: var(--esp-tools-button-color, #03a9f4);
    border: none;
    border-radius: var(--esp-tools-button-border-radius, 9999px);
  }
  button::before {
    content: " ";
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    opacity: 0.2;
    border-radius: var(--esp-tools-button-border-radius, 9999px);
  }
  button:hover::before {
    background-color: rgba(255,255,255,.8);
  }
  button:focus {
    outline: none;
  }
  button:focus::before {
    background-color: white;
  }
  button:active::before {
    background-color: grey;
  }
  :host([active]) button {
    color: rgba(0, 0, 0, 0.38);
    background-color: rgba(0, 0, 0, 0.12);
    box-shadow: none;
    cursor: unset;
    pointer-events: none;
  }
  .hidden {
    display: none;
  }`;
customElements.define("esp-web-install-button", InstallButton);
