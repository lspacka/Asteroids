function resizeCanvas() {
    let canvas = document.getElementById('canvas');
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    // Module.ccall('UpdateViewport', null, ['number', 'number'], [canvas.width, canvas.height]);
    if (Module.ccall) {
        Module.ccall('UpdateViewport', null, ['number', 'number'], [canvas.width, canvas.height]);
    }
}

  window.addEventListener('resize', resizeCanvas);
  window.addEventListener('load', resizeCanvas);