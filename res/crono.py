import numpy as np
import time
from datetime import datetime
datos = np.array([])
opt = ' '
while opt != 'q':
    opt = input()
    datos = np.append(datos, time.time())
datos = datos - datos[0]
np.savetxt(f"daticos{datetime.now()}.csv", datos);
