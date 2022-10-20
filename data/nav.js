document.write(`
<nav class="navbar navbar-expand-lg" style="background-color: transparent;">
    <div class="container-fluid">
      <a class="navbar-brand" href="#">Lámpara Iot</a>
      <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNavDropdown" aria-controls="navbarNavDropdown" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>
      <div class="collapse navbar-collapse" id="navbarNavDropdown">
        <ul class="navbar-nav">
          <li class="nav-item">
            <a class="nav-link active" aria-current="page" href="#">Inicio</a>
          </li>
          <li class="nav-item dropdown">
            <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">
              Opciones
            </a>
            <ul class="dropdown-menu" style="background-color: transparent;">
              <li><a class="dropdown-item" href="manual.html">Opción A: </a></li>
              <li><a class="dropdown-item" href="automatico.html">Opción B: </a></li>
              <li><a class="dropdown-item" href="#">Opción C: </a></li>
              <li><a class="dropdown-item" href="#">Opción D: </a></li>
              <li><a class="dropdown-item" href="#">Opción E: </a></li>
              <li><a class="dropdown-item" href="#">Opción F: </a></li>
            </ul>
          </li>
        </ul>
      </div>
    </div>
  </nav>
`);