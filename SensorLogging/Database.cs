using MySqlConnector;

namespace SensorLogging
{
    public class Database
    {
        private const string ENV_DATABASE_SERVER = "SENSOR_SERVER";
        private const string ENV_DATABASE_PORT = "SENSOR_PORT";
        private const string ENV_DATABASE_USERNAME = "SENSOR_USERNAME";
        private const string ENV_DATABASE_PASSWORD = "SENSOR_PASSWORD";
        private const string ENV_DATABASE_DATABASE = "SENSOR_DATABASE";
        private const string CONNECTION_STRING_FORMAT = "Server={0};Database={2};User Id={3};Password={4};";
        private MySqlConnection _connection;
        
        public Database()
        {
            _connection = new MySqlConnection(GetConnectionString());
        }

        private string GetEnvironmentVariable(string variable)
        {
            string value = string.Empty;
            value = System.Environment.GetEnvironmentVariable(variable) ?? string.Empty;

            return value;
        }

        private string GetConnectionString()
        {
            string result = string.Empty;

            string server = GetEnvironmentVariable(ENV_DATABASE_SERVER);
            string port = GetEnvironmentVariable(ENV_DATABASE_PORT);
            string username = GetEnvironmentVariable(ENV_DATABASE_USERNAME);
            string password = GetEnvironmentVariable(ENV_DATABASE_PASSWORD);
            string database = GetEnvironmentVariable(ENV_DATABASE_DATABASE);

            result = string.Format(CONNECTION_STRING_FORMAT, server, port, database, username, password);

            return result;
        }

        public bool SaveSensorLog(Sensor sensorLog)
        {
            bool result = false;

            _connection.Open();

            using var command = new MySqlCommand(@"spSaveLog", _connection);
            command.CommandType = System.Data.CommandType.StoredProcedure;
            command.Parameters.Add(new MySqlParameter("intSensorID_p", sensorLog.ID));
            command.Parameters.Add(new MySqlParameter("strMessage_p", sensorLog.Message));
            command.Parameters.Add(new MySqlParameter("strToken_p", sensorLog.Token));

            if (command.ExecuteNonQuery() == 1)
                result = true;

            return result;
        }
    }
}
