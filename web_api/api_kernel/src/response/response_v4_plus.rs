use serde::{Deserialize, Serialize};

use super::response_v4::ResponseV4;

#[derive(Serialize, Deserialize, Default)]
pub struct ResponseV4Plus {
    pub solutions: Vec<ResponseV4>,
}

impl ResponseV4Plus {
    pub fn new() -> Self {
        Self { solutions: Vec::new() }
    }

    pub fn from_responses(responses: Vec<ResponseV4>) -> Self {
        Self { solutions: responses }
    }

    pub fn push(&mut self, response: ResponseV4) {
        self.solutions.push(response);
    }
}
