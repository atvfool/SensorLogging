using Microsoft.AspNetCore.Mvc;
using System.Net;
using System.Security.Cryptography.X509Certificates;

namespace SensorLogging.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class SensorLoggerController : ControllerBase
    {

        [HttpPost(Name = "SaveSensorData")] 
        public JsonResult SavSensorData(Sensor sensorLog) {
            Database db = new Database();

            bool result = db.SaveSensorLog(sensorLog);
            
            return new JsonResult(new { StatusCode = HttpStatusCode.OK, data = new { data = result } });

        }
    }
}
