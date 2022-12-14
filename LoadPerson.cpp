struct DBConnectionParams {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

struct LoadPersonsQueryParams {
    int min_age;
    int max_age;
    string_view name_filter;

    LoadPersonsQueryParams& Quote(const DBHandler& db) {
        name_filter = db.Quote(name_filter);
        return *this;
    }
};

optional<DBHandler> ConnectToDB(const DBConnectionParams& params) {
    DBConnector connector(params.db_allow_exceptions, params.db_log_level);
    DBHandler db;
    if (params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params.db_name, params.db_connection_timeout);
    } else {
        db = connector.Connect(params.db_name, params.db_connection_timeout);
    }
    if (!params.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }
    return db;
}

DBQuery CreateLoadPersonsQuery(const LoadPersonsQueryParams& params) {
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << params.min_age << " and "s << params.max_age << " "s
              << "and Name like '%"s << params.name_filter << "%'"s;
    return DBQuery(query_str.str());
}

vector<Person> LoadPersons(const DBConnectionParams& db_connection_params,
                           LoadPersonsQueryParams load_persons_params) {
    auto db = ConnectToDB(db_connection_params);;
    if (!db) {
        return {};
    }

    auto query = CreateLoadPersonsQuery(load_persons_params.Quote(*db));
    vector<Person> persons;
    for (auto [name, age] : db->LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}
